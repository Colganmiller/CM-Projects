import os
import shutil
from pathlib import Path


def get_extension(filename):
    return filename.suffix.lower()  # Convert extension to lowercase

directory = Path("/Users/celligan26/Documents/testscleaner")

if not directory.exists() or not directory.is_dir():
    print(f"Directory does not exist: {directory}")
    exit()

file_list = [file for file in directory.iterdir() if file.is_file() and file.name != ".DS_Store"]

if not file_list:
    print("No files to organize")
    exit()

folders = {
    "Images": [".jpg", ".tga", ".png", ".heic"],
    "Documents": [".pdf", ".docx", ".txt", ".xlsx", ".pptx"],
    "Videos": [".mp4", ".avi", ".mov"],
    "Music": [".mp3", ".wav"],
    "Archives": [".zip", ".rar", ".tar", ".gz"],
    "Scripts": [".py", ".sh", ".bat"],
    "Others": []
}

dst_others = Path("/Users/celligan26/Documents/testscleaner/others")
dst_others.mkdir(exist_ok=True)

dst_duplicates = Path("/Users/celligan26/Documents/testscleaner/duplicates")
dst_duplicates.mkdir(exist_ok=True)

for file in file_list:
    file_ext = get_extension(file)
    move = False

    for folder_name, extensions in folders.items():
        if file_ext in extensions:
            dst = Path(f"/Users/celligan26/Documents/testscleaner/{folder_name}")
            dst.mkdir(exist_ok=True)

            dest_file = dst / file.name
            if dest_file.exists():
                print(f"Duplicate file found: {file.name}")
                shutil.move(str(file), str(dst_duplicates))
            else:
                print(f"Moving {file} to {dst}")
                shutil.move(str(file), str(dst))
            move = True
            break

    if not move:
        dest_file = dst_others / file.name
        if dest_file.exists():
            print(f"Duplicate file found: {file.name}")
            shutil.move(str(file), str(dst_duplicates))
        else:
            print(f"Moving {file} to {dst_others}")
            shutil.move(str(file), str(dst_others))


