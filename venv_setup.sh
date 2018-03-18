#!/bin/bash

if [ -d venv ]; then
    while read -ep "The venv directory already exists. Delete and recreate? [y/n]: " dandrec; do
        case ${dandrec} in
            [Yy])   break ;;
            [Nn])   exit ;;
            *)      ;;
        esac
    done
fi

rm -rf venv

python3 -m venv venv

if [ ! -f ./venv/bin/activate ]; then
    echo "Failed to create the virtualenv directory"
    exit 1
fi

. venv/bin/activate

if [ ! -x venv/bin/pip ]; then
    curl https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py | python3 -
    venv/bin/easy_install pip
    deactivate
    . venv/bin/activate
fi


python3 -V
which python3

while read -ep "Is the python3 version and location correct? [y/n]: " correctpy; do
    case ${correctpy} in
        [Yy])   break ;;
        [Nn])   exit ;;
        *)      ;;
    esac
done

pip install -r requirements.txt
CWD="$(pwd)"

pip list --format=columns
python3 -V
which python3
