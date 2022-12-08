import yaml, sys
import os

dir = sys.argv[1]
yaml_data = None
json_data = None
info = {
    'hard':         '-hardd',
    'floppy':       '-floppy',
    'sect_size':    '-ss={0}',
    'dimg':         '-dimg={0}'
}
main_path = os.path.expanduser('~')
run_command = f'.{main_path}/MocaAssembler/emu_main.o'    # ./emu_main.o will change.

def error(msg):
    print(msg)
    sys.exit(1)

if os.path.exists(dir + '/conf.yaml'):
    with open(dir + '/conf.yaml', 'r') as file:
        yaml_data = yaml.safe_load(file)
        file.close()

    if 'format' in yaml_data:

        # Drive Type. Default: hard-disk.
        if 'type' in yaml_data['format']:
            run_command += f" {info[yaml_data['format']['type']]}"
        else:
            run_command += f" {info['hard']}"

        # Sector Size. Default: 512.
        if 'sect_size' in yaml_data['format']:
            run_command += f" {info['sect_size'].format(str(yaml_data['format']['sect_size']))}"
        else:
            run_command += f" {info['sect_size'].format(str(512))}"
        
        # Disk image file. Required.
        if not 'dimg' in yaml_data['format']:
            error("`dimg` is required for emulation.\n\tthe key `dimg` referes to the disk image that the emulator will run")
        
        run_command += f" {info['dimg'].format(yaml_data['format']['dimg'])}"

        print(run_command)

        os.system(run_command)

        sys.exit(0)
    
    error("`format` information was not found in configure file")
    