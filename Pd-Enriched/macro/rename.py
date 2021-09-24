import os

def rename():

    files = os.listdir()

    for file in files:
        if('.root' in file):
            file_new = file.split('.')[0]+'_ch1.root'
            os.system('mv ' + file + ' ' +file_new)

if __name__ == '__main__':
    rename()
