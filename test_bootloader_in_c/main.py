import toml
import os, sys

def error(msg):
    print(msg)
    sys.exit(1)

if not os.path.isfile('os_info.toml'):
    error(f'Expected `os_info.toml` file in {os.getcwd()}')

loaded_info = toml.load('os_info.toml')

# Bootloader Information
version = None
name = None
bootloader_path = None
OS_exe = None

# Kernel information
kernel_path = None
kernel_linker_path = None
kernel_libs = None

# Required info from the toml file
REQUIRED_HEADERS = ['boot', 'kernel']
REQUIRED_SECTIONS = {
    'boot':['version', 'name', 'bootloader', 'bin'],
    'kernel': ['kernel_path', 'kernel_linker', 'libs', 'kernel_entry_point', 'kernel_address']
}

for i in REQUIRED_HEADERS:
    if not i in loaded_info: error(f'Expected {i} in os_info.toml')

boot_info = loaded_info[REQUIRED_HEADERS[0]]
kernel_info = loaded_info[REQUIRED_HEADERS[1]]

# Get bootloader info
for i in REQUIRED_SECTIONS['boot']:
    if not i in boot_info: error(f'Expected {i} in `boot` header of os_info.tom')

version = boot_info['version']
name = boot_info['name']
bootloader_path = boot_info['bootloader']
OS_exe = boot_info['bin']

# Get kernel info
for i in REQUIRED_SECTIONS['kernel']:
    if not i in kernel_info: error(f'Expected {i} in `kernel` header of os_info.toml')

kernel_path = kernel_info['kernel_path']
kernel_linker_path = kernel_info['kernel_linker']
kernel_libs = kernel_info['libs']

if not version == None:
    if not name == None:
        print(f'\nRunning {name}, Version {version}\n')
    else:
        print(f'\nRunning UnamedOS, Version {version}\n')
print('Bootloader: ', version, name, bootloader_path, OS_exe, '\n', 'Kernel:', kernel_path, kernel_linker_path, kernel_libs)