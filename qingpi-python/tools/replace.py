import re
import sys

filepath, search_pattern, replace_pattern = sys.argv[1], sys.argv[2], sys.argv[3]

with open(filepath, "r+") as file:
    code = file.read()
    modified_code = re.sub(search_pattern, replace_pattern, code)
    file.seek(0)
    file.write(modified_code)
    file.truncate()
