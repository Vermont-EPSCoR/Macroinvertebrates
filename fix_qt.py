#!/usr/bin/env python
import os
import subprocess
from contextlib import contextmanager


@contextmanager
def cd(path):
    cwd = os.getcwd()
    os.chdir(path)
    yield
    os.chdir(cwd)


def check_output(cmd_list):
    process = subprocess.Popen(
        cmd_list,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT
    )

    result = process.communicate()[0]
    if process.returncode != 0:
        raise Exception('`%s` failed to complete cleanly.\n\t%s' % (' '.join(cmd_list), result))

    return result


def main():
    qt_dir = '/Users/%s/Qt/5.7' % os.getenv('USER')
    os.chdir(qt_dir)

    platforms = [
        'android_armv7',
        'android_x86',
        'clang_64',
        'ios'
    ]

    # Must escape this string due the the number of regex meta characters
    original_xcrun_line = '(isEmpty.{3}list.{3}system.+find.)xcrun'
    replacement_xcrun_line = '$1xcodebuild'
    original_sdk_version = '(lessThan.QMAKE_MAC_SDK_VERSION, ")8.0'
    replacement_sdk_version = '${1}10.0'
    original_file = 'default_pre.prf'
    backup_file = original_file + '.bak'
    sdk_file = 'sdk.prf'
    sdk_file_bak = sdk_file + '.bak'

    for platform in platforms:
        default_pre_path = '%s/mkspecs/features/mac/' % platform
        with cd(default_pre_path):
            backup_exists = backup_file in os.listdir('.')

            if not backup_exists:
                print('Backing up %s to %s' % (original_file, backup_file))
                check_output([
                    'cp',
                    original_file,
                    backup_file
                ])
            else:
                print('Using backup %s as %s' % (backup_file, original_file))
                check_output([
                    'cp',
                    backup_file,
                    original_file
                ])

            cmd = [
                'perl',
                '-pi',
                '-e',
                's#%s#%s#g' % (original_xcrun_line, replacement_xcrun_line),
                original_file
            ]

            print('Running: %s' % ' '.join(cmd))
            check_output(cmd)

        if platform == 'ios':
            with cd('ios/mkspecs/macx-ios-clang/features'):
                print(os.getcwd())
                print(os.listdir('.'))
                sdk_backup_exists = sdk_file_bak in os.listdir('.')

                if not sdk_backup_exists:
                    print('Backing up %s to %s' % (sdk_file, sdk_file_bak))
                    check_output([
                        'cp',
                        sdk_file,
                        sdk_file_bak
                    ])
                else:
                    print('Using backup %s as %s' % (sdk_file_bak, sdk_file))
                    check_output([
                        'cp',
                        sdk_file_bak,
                        sdk_file
                    ])

                cmd = [
                    'perl',
                    '-pi',
                    '-e',
                    's#%s#%s#g' % (original_sdk_version, replacement_sdk_version),
                    sdk_file
                ]

                print('Upgrading iOS SDK version')
                print(' '.join(cmd))
                check_output(cmd)

                print(check_output([
                    'cat',
                    'sdk.prf'
                ]))

if __name__ == '__main__':
    main()
