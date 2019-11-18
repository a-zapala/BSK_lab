import argparse

from utils import run_cmd, read_employers


def clean_groups_and_users(employers):
    for position in employers:
        for user_id in employers[position]:
            run_cmd('deluser', user_id)
        run_cmd('groupdel', position)


parser = argparse.ArgumentParser()
parser.add_argument("file_employer_list", type=str, help="File name with employer list")
args = parser.parse_args()

employers = read_employers(args.file_employer_list)
clean_groups_and_users(employers)

directories = ['opisy_książek', 'zadania', 'raporty']
for name in directories:
    run_cmd('rm', '-rf', name)
