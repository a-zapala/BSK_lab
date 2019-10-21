import argparse
import os

from lab1.utils import run_cmd, setfacl, read_employers, create_groups_and_users

parser = argparse.ArgumentParser()
parser.add_argument("file_employer_list", type=str, help="File name with employer list")
args = parser.parse_args()

employers = read_employers(args.file_employer_list)
create_groups_and_users(employers)

directories = ['opisy_książek', 'zadania', 'raporty']

for name in directories:
    try:
        os.mkdir(name)
    except FileExistsError:
        pass
    setfacl('other', '', '---', name, '-m')
    setfacl('other', '', '---', name, '-d', '-m')

########## opisy_ksiązek #############
setfacl('group', 'sprzedawca', 'r-x', 'opisy_książek', '-m')  # enter to directory
setfacl('group', 'sprzedawca', 'r--', 'opisy_książek', '-d', '-m')  # read file in directory

setfacl('group', 'szef_sali', 'r-x', 'opisy_książek', '-m')  # enter to directory, but not
# add or delete file
setfacl('group', 'szef_sali', 'rw-', 'opisy_książek', '-d', '-m')  # read file in directory and
# modification

setfacl('group', 'dostawca', 'rwx', 'opisy_książek', '-m')  # all permissions
setfacl('group', 'dostawca', 'rwx', 'opisy_książek', '-d', '-m')  # all permissions

####zadania
setfacl('group', 'sprzedawca', 'r-x', 'zadania', '-m')  # enter to directory

for user_id in employers['sprzedawca']:
    system_user_id = int(
        run_cmd('id', '-u', user_id))  # ugly but with small integer name was interpreted as uid
    dir_name = f'zadania/{user_id}'
    try:
        os.mkdir(dir_name)
    except FileExistsError:
        pass
    setfacl('user', system_user_id, 'r-x', dir_name, '-m')  # enter to directory
    setfacl('user', system_user_id, 'r-x', dir_name, '-d', '-m')

setfacl('group', 'szef_sali', 'rwx', 'zadania', '-R', '-m')
setfacl('group', 'szef_sali', 'rwx', 'zadania', '-R', '-d', '-m')

setfacl('group', 'dostawca', 'r-x', 'zadania', '-R', '-m')
setfacl('group', 'dostawca', 'r-x', 'zadania', '-R', '-d', '-m')

###raporty ########################################################
setfacl('group', 'sprzedawca', 'r-x', 'raporty', '-m')  # enter to directory

for user_id in employers['sprzedawca']:
    system_user_id = int(
        run_cmd('id', '-u', user_id))  # ugly but with small integer name was interpreted as uid
    dir_name = f'raporty/{user_id}'
    try:
        os.mkdir(dir_name)
    except FileExistsError:
        pass
    setfacl('user', system_user_id, 'rwx', dir_name, '-m')  # enter to directory
    setfacl('user', system_user_id, 'rwx', dir_name, '-d', '-m')

setfacl('group', 'szef_sali', 'r-x', 'raporty', '-R', '-m')
setfacl('group', 'szef_sali', 'r-x', 'raporty', '-R', '-d', '-m')

setfacl('group', 'dostawca', 'r-x', 'raporty', '-R', '-m')
setfacl('group', 'dostawca', 'r-x', 'raporty', '-R', '-d', '-m')
