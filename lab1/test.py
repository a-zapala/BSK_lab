import os

from utils import read_employers, run_cmd_as_user

employers = read_employers('list')

dostawca = employers['dostawca'][0]
szef_sali = employers['szef_sali'][0]
sprzedawca = employers['sprzedawca'][0]

# a
for i in range(0, 5):
    run_cmd_as_user(dostawca, f' echo "opis książki" >  opisy_książek/{str(i)}')

# b
for i in range(0, 5):
    run_cmd_as_user(szef_sali, f'cat opisy_książek/{str(i)} > /dev/null')
    run_cmd_as_user(szef_sali, f' echo "na półce" >>  opisy_książek/{str(i)}')
run_cmd_as_user(szef_sali, 'touch opisy_książek/5 > /dev/null')
for i in range(0, 5):
    run_cmd_as_user(szef_sali, f'rm opisy_książek/{str(i)}')

# c
for i in range(0, 5):
    run_cmd_as_user(sprzedawca, f'cat opisy_książek/{str(i)}')
run_cmd_as_user(sprzedawca, f' echo "nie ma miejsca" >>  opisy_książek/4')
run_cmd_as_user(sprzedawca, f' touch  opisy_książek/5')

# d
task_path = 'zadania'
name_first_task = 'pierwsze_zadanie'
name_second_task = 'drugie_zadanie'
tasks = []

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(task_path, dir)):
        run_cmd_as_user(szef_sali, f'touch {task_path}/{dir}/{name_first_task}')
        run_cmd_as_user(szef_sali, f'touch {task_path}/{dir}/{name_second_task}')
        tasks.append(f'{task_path}/{dir}/{name_first_task}')
        tasks.append(f'{task_path}/{dir}/{name_second_task}')

# e
for task in tasks:
    run_cmd_as_user(dostawca, f'cat {task} >> /dev/null')

for task in tasks:
    run_cmd_as_user(dostawca, f'echo "unieważnione" >> {task}')

for task in tasks:
    run_cmd_as_user(dostawca, f'rm -f {task}')

# f
sellers_tasks_path = f'{task_path}/{sprzedawca}/'
run_cmd_as_user(sprzedawca, f'cat {sellers_tasks_path}/{name_first_task} >> /dev/null')
run_cmd_as_user(sprzedawca, f'cat {sellers_tasks_path}/{name_second_task} >> /dev/null')
run_cmd_as_user(sprzedawca, f'echo "nie zrobię"  >> {sellers_tasks_path}/{name_first_task}')
run_cmd_as_user(sprzedawca, f'echo "nie zrobię"  >> {sellers_tasks_path}/{name_second_task}')
run_cmd_as_user(sprzedawca, f'rm -f {sellers_tasks_path}/{name_first_task}')
run_cmd_as_user(sprzedawca, f'rm  -f {sellers_tasks_path}/{name_second_task}')

for another_seller in employers['sprzedawca']:
    if another_seller != sprzedawca:
        for f in os.listdir(f'{task_path}/{another_seller}'):
            run_cmd_as_user(sprzedawca, f'cat {task_path}/{another_seller}/{f} >> /dev/null')

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(task_path, dir)):
        run_cmd_as_user(sprzedawca, f'touch {task_path}/{dir}/nowy_plik')

# g
reports_path = 'raporty'
run_cmd_as_user(sprzedawca, f'echo "zawartosc" > {reports_path}/{sprzedawca}/{name_first_task}')
run_cmd_as_user(sprzedawca, f'echo "zawartosc" > {reports_path}/{sprzedawca}/{name_second_task}')
run_cmd_as_user(sprzedawca,
                f'x=$(cat {reports_path}/{sprzedawca}/{name_first_task}) && echo "nie zrobie:$x" >> {reports_path}/{sprzedawca}/{name_first_task}')
run_cmd_as_user(sprzedawca,
                f'x=$(cat {reports_path}/{sprzedawca}/{name_second_task}) && echo "nie zrobie:$x" >> {reports_path}/{sprzedawca}/{name_second_task}')

for another_seller in employers['sprzedawca']:
    if another_seller != sprzedawca:
        for f in os.listdir(f'{reports_path}/{another_seller}'):
            run_cmd_as_user(sprzedawca, f'echo "a kuku" >> {reports_path}/{another_seller}/{f}')

# h
for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        for f in os.listdir(f'{reports_path}/{dir}'):
            run_cmd_as_user(szef_sali, f'cat {reports_path}/{dir}/{f}')

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        for f in os.listdir(f'{reports_path}/{dir}'):
            run_cmd_as_user(szef_sali, f'echo "nie zrobione" >> {reports_path}/{dir}/{f}')

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        run_cmd_as_user(szef_sali, f'touch {reports_path}/{dir}/zobtowreszcie')

# i
for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        for f in os.listdir(f'{reports_path}/{dir}'):
            print(run_cmd_as_user(dostawca, f'cat {reports_path}/{dir}/{f}'))

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        for f in os.listdir(f'{reports_path}/{dir}'):
            run_cmd_as_user(dostawca, f'echo "podgladam" >> {reports_path}/{dir}/{f}')

for dir in os.listdir(task_path):
    if os.path.isdir(os.path.join(reports_path, dir)):
        run_cmd_as_user(dostawca, f'touch {reports_path}/{dir}/zrobtowreszcie')
