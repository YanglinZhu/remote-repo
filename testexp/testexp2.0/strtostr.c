#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int strtostr(char* str, const int strlength) {
    int nowOffset = 0;
    bool typeBeforeParentheses = false;
    bool doubleQuotationMarksBeforeParentheses = false;
    int doublestart = 0;
    int doubleend = 0;
    char tempstr[100];
    char* ptr = NULL;
    char* key[15] = {"char ", "char* " , "double ", "enum ", "float ", "int " ,
                      "long ", "short ", "signed ", "struct ", "typedef ", "union ",
                      "unsigned ", "void " , "return "};

 for(int i = 0 ; i < strlength ; i++ ) {
    if(str[i] == '"' && doubleQuotationMarksBeforeParentheses == false)
    {
      doubleQuotationMarksBeforeParentheses = true;
      doublestart = i;
    }
    if(str[i] == '"' && doubleQuotationMarksBeforeParentheses == true)
    {
      doubleend = i;
    }
  }
for (int i = 0, k = 0; i < sizeof(key) / sizeof(key[0]); i++) {
        if (strstr(str, key[i]) != NULL) {
            typeBeforeParentheses = true;
            ptr = strstr(str, key[i]);
            k = ptr - str + strlen(key[i]);
            for (int l = 0; l < strlength; l++) {
               if(l == k ){
                   tempstr[nowOffset ++] =' ';
               }else{
                  if (str[l] != ' ')
                      if((strstr(tempstr, "#include")!=NULL))
                          tempstr[nowOffset++] = str[l];
                      else if(str[l] != '\n')
                          tempstr[nowOffset++] = str[l];
                }
            }
            if(typeBeforeParentheses)
                break;
        }
    }

    if (!typeBeforeParentheses) {
        for (int i = 0; i < strlength ; ++i) {
            if(doubleQuotationMarksBeforeParentheses == false){
                if (str[i] != ' ' ) {
                    if((strstr(tempstr, "#include")!=NULL)||str[i] != '\n')
                    tempstr[nowOffset++] = str[i];
                }
            }else {
                  if(i < doublestart || i > doubleend)
                  {
                    if (str[i] != ' ' ) {
                        if((strstr(tempstr, "#include")!=NULL)||str[i] != '\n')
                            tempstr[nowOffset++] = str[i];
                        }
                  }else tempstr[nowOffset++] = str[i];
            }
        }
    }

    for (int i = 0; i < nowOffset ; i++) {
                str[i] = tempstr[i];
    }

    return nowOffset;
}

