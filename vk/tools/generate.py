#!/usr/bin/env python3
import argparse
import shutil
import re
import xml.etree.ElementTree as ET
from pathlib import Path

SYSTEMS = {
    'VkInstance': 'instance',
    'VkPhysicalDevice': 'physical_device',
    'VkDevice': 'device',
    'VkQueue': 'queue',
    'VkCommandBuffer': 'command',
}

def api_filter(el):
    api = el.get('api')
    return not api or 'vulkan' in api.split(',')

def guarded_platforms(root):
    return {p.get('name'): p.get('protect') for p in root.findall('./platforms/platform')}

def guards(root):
    platforms = guarded_platforms(root)
    protection = {}
    availability = {}
    for feature in root.findall('./feature'):
        if not api_filter(feature):
            continue
        for req in feature.findall('require'):
            if not api_filter(req):
                continue
            guard = req.get('protect')
            for tag in ('command', 'type', 'enum'):
                for x in req.findall(tag):
                    name = x.get('name')
                    if not name or not api_filter(x):
                        continue
                    availability[name] = True
                    protection.setdefault(name, set()).add((guard,) if guard else None)
    for extension in root.findall('./extensions/extension'):
        if 'vulkan' not in (extension.get('supported') or '').split(','):
            continue
        platform = platforms.get(extension.get('platform'))
        base = set()
        if platform:
            base.add(platform)
        if extension.get('provisional') == 'true':
            base.add('VK_ENABLE_BETA_EXTENSIONS')
        for req in extension.findall('require'):
            if not api_filter(req):
                continue
            cond = set(base)
            if req.get('protect'):
                cond.add(req.get('protect'))
            for tag in ('command', 'type', 'enum'):
                for x in req.findall(tag):
                    name = x.get('name')
                    if not name or not api_filter(x):
                        continue
                    availability[name] = True
                    protection.setdefault(name, set()).add(tuple(sorted(cond)) if cond else None)
    return availability, protection

def guard_expr(values):
    if not values or None in values:
        return None
    return ' || '.join('(' + ' && '.join('defined(' + s + ')' for s in group) + ')'
                      for group in sorted(values))

def signature(command, lookup, visited=None):
    if visited is None:
        visited = set()
    if command.get('alias'):
        alias = command.get('alias')
        if alias in visited or alias not in lookup:
            return None
        resolved = signature(lookup[alias], lookup, visited | {alias})
        if resolved is None:
            return None
        _, result, params = resolved
        return command.get('name'), result, params
    proto = command.find('proto')
    if proto is None:
        return None
    name, result = proto.findtext('name'), proto.findtext('type')
    if not name or not result:
        return None
    params = []
    for param in command.findall('param'):
        if not api_filter(param):
            return None
        param_name = param.findtext('name')
        if not param_name:
            return None
        params.append((param.findtext('type'), ''.join(param.itertext()).strip(), param_name))
    return name, result, params

def exposed_constants(include_root):
    """Read real C-header declarations, not only vk.xml registry availability."""
    names = set()
    for directory in ('vulkan', 'vk_video'):
        folder = include_root / directory
        if not folder.is_dir():
            continue
        for header in folder.glob('*.h'):
            source = header.read_text(encoding='utf-8', errors='replace')
            names.update(re.findall(r'(?m)^\s*#\s*define\s+(VK_[A-Z0-9_]+)\b', source))
            names.update(re.findall(r'(?m)^\s*(VK_[A-Z0-9_]+)\s*(?:=|,)', source))
    return names

def regenerate(root, declared_constants=None):
    available, protected = guards(root)
    commands = root.findall('./commands/command')
    by_name = {}
    for command in commands:
        name = command.get('name') or command.findtext('./proto/name')
        if name:
            by_name[name] = command
    sections = {s: [] for s in ('global', 'instance', 'physical_device',
                                'device', 'queue', 'command')}
    seen = set()
    for command in commands:
        if not api_filter(command):
            continue
        sig = signature(command, by_name)
        if sig is None:
            continue
        name, result, params = sig
        if name in seen or (available and name not in available):
            continue
        seen.add(name)
        scope = SYSTEMS.get(params[0][0], 'global') if params else 'global'
        func = name[2].lower() + name[3:]
        if scope == 'command' and name.startswith('vkCmd'):
            func = name[5].lower() + name[6:]
        elif scope == 'queue' and name.startswith('vkQueue'):
            func = name[7].lower() + name[8:]
        declarations, arguments = [], []
        for i, (_, raw, old_name) in enumerate(params):
            new_name = f'a{i}'
            changed = re.sub(r'\b' + re.escape(old_name) + r'\b', new_name, raw)
            if changed == raw:
                raise ValueError(f'Parameter missing: {name}, {raw}')
            declarations.append(changed)
            arguments.append(new_name)
        return_stmt = '' if result == 'void' else 'return '
        body = f'inline {result} {func}(' + ', '.join(declarations) + ') {\n'
        body += f'    auto fn = VULKAN_HPP_DEFAULT_DISPATCHER.{name};\n'
        body += f'    if (!fn) throw std::runtime_error("{name} is unavailable");\n'
        body += '    ' + return_stmt + 'fn(' + ', '.join(arguments) + ');\n}'
        protection = guard_expr(protected.get(name))
        if protection:
            body = '#if ' + protection + '\n' + body + '\n#endif'
        sections[scope].append((func, body))
    type_lines = []
    seen_types = set()
    for typ in root.findall('./types/type'):
        if not api_filter(typ):
            continue
        name = typ.get('name') or typ.findtext('name')
        if not name or name in seen_types:
            continue
        if not (name.startswith('Vk') or name.startswith('PFN_vk')):
            continue
        if available and name not in available and typ.get('category') not in ('basetype', 'funcpointer'):
            continue
        seen_types.add(name)
        exported = name[2:] if name.startswith('Vk') else name
        alias = f'using {exported} = ::{name};'
        protection = guard_expr(protected.get(name))
        if protection:
            alias = '#if ' + protection + '\n' + alias + '\n#endif'
        type_lines.append(alias)
    constants = []
    seen_constants = set()
    for enum in root.iter('enum'):
        name = enum.get('name')
        if not name or name in seen_constants or not api_filter(enum):
            continue
        if not re.fullmatch(r'VK_[A-Z_0-9]+', name):
            continue
        if available and name not in available:
            continue
        if declared_constants is not None and name not in declared_constants:
            continue
        if not (name[3].isalpha() or name[3] == '_'):
            continue
        seen_constants.add(name)
        constant = f'inline constexpr auto {name[3:]} = {name};'
        protection = guard_expr(protected.get(name))
        if protection:
            constant = '#if ' + protection + '\n' + constant + '\n#endif'
        constants.append(constant)
    output = [
        '#pragma once', '#include "../common.hpp"', '#include <stdexcept>',
        '', 'namespace vk::api::types {', *type_lines, '}', '',
        'namespace vk::api::constants {', *constants, '}', ''
    ]
    manifest = []
    for scope, functions in sections.items():
        output.append(f'namespace vk::api::{scope} {{')
        output.extend(body for _, body in functions)
        output.append('}')
        output.append('')
        manifest.extend(f'    vk::api::{scope}::{name}(...)' for name, _ in functions)
    return '\n'.join(output), manifest, len(seen), len(seen_types)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('registry')
    parser.add_argument('header')
    parser.add_argument('source')
    parser.add_argument('--readme')
    parser.add_argument('--headers')
    parser.add_argument('--vendor')
    args = parser.parse_args()
    root = ET.parse(args.registry).getroot()
    declared = exposed_constants(Path(args.headers)) if args.headers else None
    code, manifest, commands, types = regenerate(root, declared)
    h = Path(args.header)
    h.parent.mkdir(parents=True, exist_ok=True)
    h.write_text(code)
    cpp = Path(args.source)
    cpp.parent.mkdir(parents=True, exist_ok=True)
    cpp.write_text('#include "generated.hpp"\n')
    if args.headers and args.vendor:
        include_root = Path(args.headers)
        vendor_root = Path(args.vendor)
        for directory in ('vulkan', 'vk_video'):
            source = include_root / directory
            if not source.is_dir():
                raise FileNotFoundError(str(source))
            shutil.copytree(source, vendor_root / directory, dirs_exist_ok=True)
    if args.readme:
        dest = Path(args.readme)
        prior = dest.read_text() if dest.exists() else ''
        prior = prior.split('\ncomplete Vulkan commands\n', 1)[0].rstrip()
        generated = 'complete Vulkan commands\n' + '\n'.join(manifest) + '\n'
        dest.write_text(prior + '\n\n' + generated if prior else generated)
    print(f'Generated {commands} commands, {types} type aliases')

if __name__ == '__main__':
    main()
