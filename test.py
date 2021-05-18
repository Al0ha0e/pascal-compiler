import os

for root, _, files in os.walk('./tests/'):
    for file in files:
        print(">>>>>>>TESTING ", root+file)
        # nfile = file.replace(' ', '_')
        # os.rename(root+file, root+nfile)
        os.system('compiler.exe '+root+file)
        print('\n')
        print('\n')
