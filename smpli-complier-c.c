#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void trim(char *str) {
    int l = strlen(str);
    while (l > 0 && isspace((unsigned char)str[l - 1])) str[--l] = '\0';
    int i = 0;
    while (str[i] && isspace((unsigned char)str[i])) i++;
    if (i > 0) memmove(str, str + i, strlen(str + i) + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    if (!strstr(argv[1], ".simp")) return 1;

    FILE *source = fopen(argv[1], "r");
    if (!source) return 1;

    FILE *out = fopen("temp_output.c", "w");
    fprintf(out, "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <stdbool.h>\n");
    fprintf(out, "char _current_msg[256] = \"\"; char* _pressed_key = NULL; int _xyz[3] = {0,0,0};\n");
    fprintf(out, "char _data_type[64] = \"\"; char _hint_text[256] = \"\";\n");
    fprintf(out, "char _valid_types[20][32] = {\"3Dmodel\",\"2DSprite\",\"textDocument\",\"script\",\"pythonScript\",\"3Dprefab\",\"2Dprefab\",\"3Dscene\",\"2Dscene\",\"picturePNG\",\"pictureJPG\",\"videoMP4\",\"videoMOV\",\"musicMP3\",\"musicOGG\"};\n");
    fprintf(out, "int _types_count = 15;\n");

    char line[512];
    int indent = 0;

    while (fgets(line, sizeof(line), source)) {
        trim(line);
        if (strlen(line) == 0) continue;

        if (strcmp(line, "{") == 0) {
            for (int i = 0; i < indent; i++) fprintf(out, "    ");
            fprintf(out, "{\n");
            indent++;
            continue;
        }
        if (strcmp(line, "}") == 0) {
            indent = (indent > 0) ? indent - 1 : 0;
            for (int i = 0; i < indent; i++) fprintf(out, "    ");
            fprintf(out, "}\n");
            continue;
        }

        for (int i = 0; i < indent; i++) fprintf(out, "    ");

        if (strcmp(line, "start") == 0) {
            fprintf(out, "int main()\n");
            continue;
        }
        if (strcmp(line, "data") == 0) continue;
        if (strcmp(line, "stop") == 0) {
            fprintf(out, "return 0;\n");
            continue;
        }
        if (strncmp(line, "scriptTag(", 10) == 0 || strncmp(line, "use(", 4) == 0) continue;

        if (strncmp(line, "addDataType(", 12) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "strcpy(_valid_types[_types_count++], \"%s\");\n", p);
            continue;
        }
        if (strncmp(line, "dataType(", 9) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "for(int i=0;i<_types_count;i++){if(strcmp(_valid_types[i],\"%s\")==0){strcpy(_data_type,\"%s\");break;}}\n", p, p);
            continue;
        }
        if (strncmp(line, "print(", 6) == 0 || strncmp(line, "putText(", 8) == 0 || strncmp(line, "alert(", 6) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            if (p[0] == '"') fprintf(out, "printf(\"%%s\\n\", %s);\n", p);
            else fprintf(out, "printf(\"%%d\\n\", %s);\n", p);
            continue;
        }
        if (strncmp(line, "putNum(", 7) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "printf(\"%%d\\n\", %s);\n", p);
            continue;
        }
        if (strncmp(line, "printHex(", 9) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "printf(\"0x%%X\\n\", %s);\n", p);
            continue;
        }
        if (strncmp(line, "printBinary(", 12) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "for(int i=31;i>=0;i--)printf(\"%%d\",(%s>>i)&1);printf(\"\\n\");\n", p);
            continue;
        }
        if (strcmp(line, "alertSound()") == 0) {
            fprintf(out, "printf(\"\\a\");fflush(stdout);\n");
            continue;
        }
        if (strncmp(line, "hint(", 5) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "sprintf(_hint_text, \"%%s\", %s);\n", p);
            continue;
        }
        if (strncmp(line, "ccor(", 5) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "int _c[3]={%s};_xyz[0]=_c[0];_xyz[1]=_c[1];_xyz[2]=_c[2];\n", p);
            continue;
        }
        if (strncmp(line, "check(", 6) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "(bool)(%s);\n", p);
            continue;
        }
        if (strncmp(line, "readData(", 9) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "FILE* f=fopen(%s,\"r\");if(f){char c;while((c=fgetc(f))!=EOF)putchar(c);fclose(f);}\n", p);
            continue;
        }
        if (strncmp(line, "deleteData(", 11) == 0) {
            char *p = strchr(line, '(') + 1;
            char *e = strrchr(p, ')');
            if (e) *e = '\0';
            fprintf(out, "remove(%s);\n", p);
            continue;
        }

        char *eq = strchr(line, '=');
        if (eq && strchr(line, '(') && strchr(line, ')')) {
            *eq = '\0';
            char *var_name = line;
            char *val_part = eq + 1;
            trim(var_name);
            trim(val_part);
            char *op = strchr(val_part, '(') + 1;
            char *cl = strrchr(val_part, ')');
            if (cl) *cl = '\0';
            if (var_name[strlen(var_name) - 1] == ')') {
                char *v_op = strchr(var_name, '(');
                if (v_op) *v_op = '\0';
                trim(var_name);
            }
            if (op[0] == '"') fprintf(out, "char* %s = %s;\n", var_name, op);
            else fprintf(out, "int %s = %s;\n", var_name, op);
            continue;
        }

        if (strncmp(line, "if ", 3) == 0) {
            char *cond_start = strchr(line, '(') + 1;
            char *cond_end = strrchr(line, ')');
            if (cond_end) *cond_end = '\0';
            fprintf(out, "if (%s)\n", cond_start);
            continue;
        }
        if (strcmp(line, "else") == 0) {
            fprintf(out, "else\n");
            continue;
        }
    }

    fclose(source);
    fclose(out);

    char cmd[256];
    char out_name[128];
    strcpy(out_name, argv[1]);
    char *dot = strrchr(out_name, '.');
    if (dot) *dot = '\0';

    sprintf(cmd, "clang temp_output.c -o %s && rm temp_output.c", out_name);
    system(cmd);

    return 0;
}
