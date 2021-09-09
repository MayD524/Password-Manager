import os

def get_cpp_files(sdir:str, ignore=[]) -> str:
    cpp_files = ""
    for file in os.listdir(sdir):
        if os.path.isfile(file) and file.endswith(".cpp"):
            if file in ignore:
                continue
            cpp_files += f" {sdir}/{file}"
        elif os.path.isdir(file):
            cpp_files += f" {get_cpp_files(file, ignore)}"
    
    return cpp_files

if __name__ == "__main__":
    cpp_files = get_cpp_files(os.getcwd(), ["test.cpp"])
    os.system(f"g++ -std=c++11{cpp_files} -o main.exe")
    #os.system(".\\main.exe > ./logs/run.txt")