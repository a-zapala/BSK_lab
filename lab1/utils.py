import os
import pwd
import getpass
import subprocess


def run_cmd(*args):
    cmd = ' '.join(list(args))
    user = getpass.getuser()
    return run_cmd_as_user(user, cmd)


def demote(user_uid, user_gid):
    def result():
        os.setgid(user_gid)
        os.setuid(user_uid)

    return result


def run_cmd_as_user(user_name, cmd):
    pw_record = pwd.getpwnam(user_name)
    user_uid = pw_record.pw_uid
    user_gid = pw_record.pw_gid
    try:
        return subprocess.check_output(cmd, preexec_fn=demote(user_uid, user_gid),
                                       shell=True).decode("utf-8")
    except subprocess.CalledProcessError as e:
        return e


def read_employers(file_name):
    employers = {'sprzedawca': [], 'dostawca': [], 'szef_sali': []}

    with open(file_name) as fp:
        for line in fp:
            records = line.strip().split(' ')
            if len(records) != 4:
                raise SyntaxError("Wrong number of parameter")

            employer_id = records[0]
            position = records[3]
            int(employer_id)

            if position in employers:
                employers[position].append(employer_id)
            else:
                raise SyntaxError('Not recognized position')

    if len(employers['sprzedawca']) < 2 or len(employers['dostawca']) != 1 or len(
            employers['szef_sali']) != 1:
        raise SyntaxError("Wrong amount of employers")

    return employers


def create_groups_and_users(employers):
    for position in employers:
        run_cmd('groupadd', position)
        for user_id in employers[position]:
            run_cmd('useradd', '-g', position, user_id)
            run_cmd('usermod', '--password', '1U2891V.qj3/w', user_id)


def setfacl(type, id, permissions, path, *flags):
    acl_str = f'{type}:{id}:{permissions}'
    run_cmd('setfacl', *flags, acl_str, path)
