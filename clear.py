import os
import shutil

files = ['Pixelate.sln','Pixelate.VC.db']
folders = ['Intermediate', 'Binaries', 'Saved']

for file in files:
    if os.path.exists(file):
        os.remove(file)

for folder in folders:
    if os.path.exists(folder):
        shutil.rmtree(folder)