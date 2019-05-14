import argparse
import sys
import os
import shutil


def parse_args():
    """
    Parse input arguments
    """
    parser = argparse.ArgumentParser(
                        description='Copy all figures from the folder')
    parser.add_argument('--video_folder', dest='video_list_file',
                        help='a file that contains all videos (one video per line)',
                        default=None, type=str)
    parser.add_argument('--output_folder', dest='output_folder',
                        help='output folder to save selected video frames (root of the dataset)',
                        default=None, type=str)

    parser.add_argument('--type', dest  = 'extIn', help = 'readin picture types',
            type = str)
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()
    return args

if __name__ == '__main__':

    args = parse_args()
    print('Called with args:')
    print(args)

    fileType = args.extIn
    dest_dir = args.output_folder
    if dest_dir[-1] != "/":
        dest_dir += "/"
    allFiles = []
    rootdir = args.video_list_file
    for subdir, dirs, files in os.walk(rootdir):
        for file in files:
            #print(os.path.join(subdir, file))
            allFiles.append(os.path.join(subdir, file))
    print(allFiles)
    trim_prefix = len(dest_dir)
    for file in allFiles:
        if file.endswith(fileType):
            shutil.copy2(file, dest_dir)
            dest_file = os.path.join(dest_dir, os.path.basename(file))
            #print(file)
            print(dest_file)
            rename = dest_dir + os.path.abspath(file)[trim_prefix:].replace("/", ".")[1:]
            print(rename)
os.rename(dest_file, rename)
