import sys
import re
import os

def translate_to_python(filename):
    if not filename.endswith('.simp'):
        return None
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except FileNotFoundError:
        return None

    VALID_DATA_TYPES = [
        "3Dmodel", "2DSprite", "textDocument", "script", "pythonScript",
        "3Dprefab", "2Dprefab", "3Dscene", "2Dscene", "picturePNG",
        "pictureJPG", "videoMP4", "videoMOV", "musicMP3", "musicOGG"
    ]

    py_lines = [
        "import sys", 
        "import os", 
        "_current_msg = ''",
        "_pressed_key = None",
        "_xyz =",
        "_data_type = None",
        "_hint_text = ''",
        ""
    ]
    indent_level = 0

    for line in lines:
        line = line.strip()
        if not line:
            continue

        if line == "{":
            indent_level += 1
            continue
        if line == "}":
            indent_level = max(0, indent_level - 1)
            continue

        indent = "    " * indent_level

        if line == "start" or line == "data":
            continue
        if line == "stop":
            py_lines.append(f"{indent}sys.exit(0)")
            continue
        if line.startswith("scriptTag(") or line.startswith("use("):
            continue

        if line.startswith("addDataType(") and line.endswith(")"):
            new_type = line[12:-1].strip()
            VALID_DATA_TYPES.append(new_type)
            continue

        if line.startswith("dataType(") and line.endswith(")"):
            dt = line[9:-1].strip()
            matched_type = next((t for t in VALID_DATA_TYPES if t.lower() == dt.lower()), dt)
            py_lines.append(f"{indent}_data_type = '{matched_type}'")
            continue

        if line.startswith("print(") and line.endswith(")"):
            py_lines.append(f"{indent}print({line[6:-1]})")
            continue
        if line.startswith("printHex(") and line.endswith(")"):
            py_lines.append(f"{indent}print(hex(int({line[9:-1]})))")
            continue
        if line.startswith("printBinary(") and line.endswith(")"):
            py_lines.append(f"{indent}print(bin(int({line[12:-1]})))")
            continue
        if line.startswith("alert(") and line.endswith(")"):
            py_lines.append(f"{indent}print({line[6:-1]})")
            continue
        if line == "alertSound()":
            py_lines.append(f"{indent}print('\\a', end='', flush=True)")
            continue

        if line.startswith("putText(") and line.endswith(")"):
            py_lines.append(f"{indent}print(str({line[8:-1]}), end='\\n')")
            continue
        if line.startswith("putNum(") and line.endswith(")"):
            py_lines.append(f"{indent}print(str({line[7:-1]}), end='\\n')")
            continue
        if line.startswith("hint(") and line.endswith(")"):
            py_lines.append(f"{indent}_hint_text = str({line[5:-1]})")
            continue

        var_match = re.match(r"([a-zA-Z0-9_().]+)\s*=\s*\((.*)\)", line)
        if var_match:
            left_side = var_match.group(1).replace("()", "").strip()
            expression = var_match.group(2).strip()
            py_lines.append(f"{indent}{left_side} = {expression}")
            continue

        if line.startswith("if ") and ("then" in line or "do" in line or "toDo()" in line):
            cond_match = re.search(r"if\s+\((.*)\)\s+(then|do|toDo\(\))", line)
            if cond_match:
                condition = cond_match.group(1)
                py_lines.append(f"{indent}if {condition}:")
                continue
        if line == "else":
            py_lines.append(f"{"    " * (indent_level - 1)}else:")
            continue

        if line.startswith("check(") and line.endswith(")"):
            py_lines.append(f"{indent}bool({line[6:-1]})")
            continue
        if line.startswith("ccor(") and line.endswith(")"):
            coords = line[5:-1].strip()
            if coords:
                py_lines.append(f"{indent}_xyz = [{coords}]")
            else:
                py_lines.append(f"{indent}_xyz")
            continue

        if "whenPressed(" in line and ")Do(" in line:
            btn_match = re.search(r"whenPressed\((.*)\)Do\((.*)\)", line)
            if btn_match:
                key = btn_match.group(1).strip()
                action = btn_match.group(2).strip()
                py_lines.append(f"{indent}if _pressed_key == {key or 'None'}: {action}")
            continue

        if "sendMessage(" in line and ")To(" in line:
            msg_match = re.search(r"sendMessage\((.*)\)To\((.*)\)", line)
            if msg_match:
                msg = msg_match.group(1).strip()
                py_lines.append(f"{indent}_current_msg = {msg}")
            continue
        if line == "givenMessage()":
            py_lines.append(f"{indent}_current_msg")
            continue

        if line.startswith("readData(") and line.endswith(")"):
            target_file = line[9:-1].strip()
            py_lines.append(f"{indent}if os.path.exists({target_file}):")
            py_lines.append(f"{indent}    with open({target_file}, 'r') as f: print(f.read())")
            continue
        if line.startswith("deleteData(") and line.endswith(")"):
            target_file = line[11:-1].strip()
            py_lines.append(f"{indent}if os.path.exists({target_file}): os.remove({target_file})")
            continue
        if "moveData(" in line:
            mv_match = re.search(r"moveData\((.*)\)", line)
            if mv_match:
                parts = mv_match.group(1).split(',')
                if len(parts) == 2:
                    py_lines.append(f"{indent}if os.path.exists({parts.strip()}): os.rename({parts.strip()}, {parts.strip()})")
            continue

    return "\n".join(py_lines)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        py_code = translate_to_python(sys.argv)
        if py_code:
            exec(py_code, {})
            