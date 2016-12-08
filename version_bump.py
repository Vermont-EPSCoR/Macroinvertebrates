#!/usr/bin/env python
import subprocess
import re
import os


def perl_pie(search, replace, f):
    cmd = [
        'perl',
        '-pi.bak',
        '-e',
        's/%s/%s/g' % (search, replace),
        f
    ]

    subprocess.check_call(cmd)


def main():
    data = ''
    version_name_pattern = '([0-9]+.[0-9]+)'
    version_code_pattern = '([0-9]+)'
    version_number_ui_chunk = '<string>version: %s<\/string>'
    version_number_android_version_name_chunk = 'android:versionName="%s"'
    version_number_android_version_code_chunk = 'android:versionCode="%s"'

    with open('android/AndroidManifest.xml', 'r') as input_file:
        data = input_file.read()

    if not data:
        print('Unable to open android/AndroidManifest.xml. Exiting 1.')
        exit(1)

    version_numbers = re.findall(
        version_number_android_version_name_chunk % version_name_pattern,
        data
    )

    build_version = re.findall(
        version_number_android_version_code_chunk % version_code_pattern,
        data
    )

    major_version = str(int(version_numbers[0].split('.')[0]))
    build_version = str(int(build_version[0]) + 1)

    new_version = '%s.%s' % (
        major_version,
        build_version
    )

    print('Running perl pie.')
    try:
        perl_pie(
            version_number_ui_chunk % version_name_pattern,
            version_number_ui_chunk % new_version,
            'views/homeview.ui'
        )

        perl_pie(
            version_number_android_version_name_chunk % version_name_pattern,
            version_number_android_version_name_chunk % new_version,
            'android/AndroidManifest.xml'
        )

        perl_pie(
            version_number_android_version_code_chunk % version_code_pattern,
            version_number_android_version_code_chunk % build_version,
            'android/AndroidManifest.xml'
        )
    except:
        print('One or more of the perl pies failed. Exiting 1.')
        exit(1)

    print('Removing backup files')
    os.unlink('views/homeview.ui.bak')
    os.unlink('android/AndroidManifest.xml.bak')

    print('Fin')
    exit(0)


if __name__ == '__main__':
    main()
