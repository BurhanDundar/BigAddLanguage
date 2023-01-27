#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

char *Variables[1000]; int vIndex = 0;
int Integers[1000] = { 0 }; int iIndex = 0;

int loopInt;
char loopIntWord[100];
bool isInLoop = false;

bool isKeyword(char* str);
bool isInteger(char* str);
bool isIntegerVariable(char* str);
bool isVariable(char* str);
bool isVariable2(char* str,FILE* analyseFile);
bool isStringConstant(char *str,FILE* analyseFile);
bool isStringConstantParse(char *str,FILE* analyseFile);
void lexicalAnalyse(char* code,FILE* analyseFile);
int substring(char* source, int from, int to, char* target,FILE* analyseFile);
void isComma(char* str,FILE* analyseFile);
void isLexicalLoop(char* str,FILE* analyseFile);
void isPoint(char* str,FILE* analyseFile);
void isCommentLine(char* str,FILE* analyseFile);
void isLoop(char* str,FILE* analyseFile);
void parse(char* code,FILE* analyseFile);
void isParseLoop(char* str,FILE* analyseFile);

char strConstant[100]; char strConstant2[100];
bool isStringConstant(char *str,FILE* analyseFile){
	bool trueStringConstant = false;
		int j;
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] == '"') {
				for (j = i+1; j < strlen(str); j++) {
					if (str[j] == '"') {
						substring(str, i, j - i + 1, strConstant,analyseFile);
						substring(str, i+1, j - i-1, strConstant2,analyseFile);
						trueStringConstant = true;
					}
				}
			}
		}
		return trueStringConstant;
}

char strConstantParse[100]; char strConstantParse2[100];
bool isStringConstantParse(char *str,FILE* analyseFile){
	bool trueStringConstantParse = false;
		int j;
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] == '"') {
				for (j = i+1; j < strlen(str); j++) {
					if (str[j] == '"') {
						substring(str, i, j - i + 1, strConstantParse,analyseFile);
						substring(str, i+1, j - i-1, strConstantParse2,analyseFile);
						trueStringConstantParse = true;
						printf("%s",strConstantParse2);
					}
				}
			}
		}
		return trueStringConstantParse;
}

int substring(char* source, int from, int to, char* target,FILE* analyseFile);
char loopLine[100]; char c[100]; char d[100];
void isLoop(char* str,FILE* analyseFile) {
	bool trueLoop = false;
	int j;
	for(loopInt;0<loopInt;loopInt--){
		isInLoop = true;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '[') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == ']') {
					substring(str, i + 1 , j - i -1, loopLine,analyseFile);
					parse(loopLine,analyseFile);
					substring(str, 0, i, c,analyseFile);
					substring(str, j + 1, strlen(str), d,analyseFile);
					trueLoop = true;

				}
			}

			if (!trueLoop) {
				printf("Loop Line is not completed.\n");
				exit(0);
			}
		}

	}
	for (int i = 0; i < vIndex; i++) {
	if (!strcmp(Variables[i], loopIntWord)) {
		Integers[i]-=1;
		break;
	}
}
}
	isInLoop = false;
}

void isLoop(char* str,FILE* analyseFile);
void parse(char* code,FILE* analyseFile) { // Most important function for interpreter(parse)
	int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(code, ".");

	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, "."); //split code according to "."(point) .

	}

	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char* word[1000];
		char* splitLine = strtok(line[i], " \n\r[]\""); //all line until pointer see the "."(point).
		while (splitLine != NULL) {
			word[wordCount++] = splitLine;
			splitLine = strtok(NULL, " \n\r");
		}

		if (wordCount == 0) { return; } // if there is no word, do nothing.

		//Int Keyword
		if (!strcmp(word[0], "int")) { //if first word in line is int ,we observe the rules for int.
			//Variable
			if (word[1] == NULL) {
				printf("Variable is expected.\n");
				exit(0);
			}
			else if (isKeyword(word[1])) { //look for the word is keyword or not.
				printf("%s is a keyword. Variable is expected. \n",word[1]);
				exit(0);

			}
			else if (isVariable(word[1])) {	//look for the word is variable or not.
				for(int i = 0;i<vIndex;i++){
					if(!strcmp(word[1],Variables[i])){
						printf("%s is identified before!",word[1]);
						exit(0);
					}
				}
				Variables[vIndex++] = word[1];
				Integers[iIndex++] = 0;
			}

			else {
				printf("%s is not a variable. Variable is expected.\n", word[1]);
				exit(0);

			}



			//End Of Line
			if (wordCount == 2) {
				//do nothing
				//it is what should be
			}
			else {
				printf("End of line is expected.\n");
				exit(0);
			}
		}

		//Move Keyword
		else if (!strcmp(word[0], "move")) { //if first word in line is move ,we observe the rules for move.

			//Integer Or Variable
			int moveInteger = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected.\n");
				exit(0);

			}
			else if (isKeyword(word[1])) {
				printf("%s is a keyword. Integer or variable are expected.\n", word[1]);
				exit(0);

			}
			else if (isInteger(word[1])) { //look for the word is integer or not.
				moveInteger = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {  //look for the word is integerVariable or not.
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
						moveInteger = Integers[i];
						break;
					}
				}
			}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
				printf("%s is not identified before!",word[1]);
				exit(0);
		}
			else {
				printf("%s is not a integer or variable. Integer or variable are expected.\n", word[1]);
				exit(0);

			}

			//To Keyword
			if (word[2] == NULL) {
				printf("Keyword 'to' is expected.\n");
				exit(0);

			}
			else if (!strcmp(word[2], "to")) {
				//do nothing
				//it is what should be
			}
			else {
				printf("Keyword 'to' is expected.\n");
				exit(0);

			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected.\n");
				exit(0);

			}
			else if (isKeyword(word[3])) {
				printf("%s is a keyword. Variable is expected.\n", word[3]);
				exit(0);

			}
			else if (isIntegerVariable(word[3])) {
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[3])) {
						Integers[i] = moveInteger;
						break;
					}
				}
			}else if(isVariable(word[3]) && !isIntegerVariable(word[3])){ //controls the word is identified before or not.
				printf("%s is not identified before!",word[3]);
				exit(0);
			}
			else {
				printf("%s is not a variable. Variable is expected.\n", word[3]);
				exit(0);

			}

			//End Of Line
			if (wordCount == 4) {
				//do nothing
				// it is what should be
			}
			else {
				printf("End of line is expected.\n");
				exit(0);

			}
		}


		//Add Keyword
		else if (!strcmp(word[0], "add")) { //if first word in line is add ,we observe the rules for add.

			//Integer Or Variable
			int addInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected.\n");
				exit(0);

			}
			else if (isKeyword(word[1])) {
				printf("%s is a keyword. Integer or variable are expected.\n", word[1]);
				exit(0);

			}
			else if (isIntegerVariable(word[1])) {

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
						addInt = Integers[i];
						break;
					}
				}
			}
			else if (isInteger(word[1])) {

				addInt = atoi(word[1]);

			}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
				printf("%s is not identified before!",word[1]);
				exit(0);
		}
			else {
				printf("%s is not a integer or variable. Integer or variable are expected.\n", word[1]);
				exit(0);
			}

			//To Keyword
			if (word[2] == NULL) {
				printf("Keyword 'to' is expected.\n");
				exit(0);
			}
			else if (!strcmp(word[2], "to")) {
				//do nothing
				//it is what should be
			}
			else {
				printf("Keyword 'to' is expected.\n");
				exit(0);
			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected.\n");
				exit(0);
			}
			else if (isKeyword(word[3])) {
				printf("%s is a keyword. Variable is expected.\n", word[3]);
				exit(0);
			}
			else if (isIntegerVariable(word[3])) {
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[3])) {
						Integers[i] += addInt;
						break;
					}
				}

			}else if(isVariable(word[3]) && !isIntegerVariable(word[3])){
				printf("%s is not identified before!",word[3]);
				exit(0);
		}
			else {
				printf("%s is not a variable. Variable is expected.\n", word[3]);
				exit(0);
			}

			//End Of Line
			if (wordCount == 4) {
				//do nothing
				// it is what should be
			}
			else {
				printf("End of line is expected.\n");
				exit(0);
			}
		}



		//Sub Keyword
		else if (!strcmp(word[0], "sub")) { //if first word in line is sub ,we observe the rules for sub.

			//Integer Or Variable
			int subInt = 0;
			if (word[1] == NULL) {
				printf("Integer or variable are expected.\n");
				exit(0);
			}
			else if (isKeyword(word[1])) {
				printf("%s is a keyword. Integer or variable are expected.\n", word[1]);
				exit(0);
			}else if (isIntegerVariable(word[1])) {
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
						subInt = Integers[i];
						break;
					}
				}
			}
			else if (isInteger(word[1])) {
				subInt = atoi(word[1]);
			}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
				printf("%s is not identified before!",word[1]);
				exit(0);
		}
			else {
				printf("%s is not a integer or variable. Integer or variable are expected.\n", word[1]);
				exit(0);
			}

			//From Keyword
			if (word[2] == NULL) {
				printf("Keyword 'from' is expected.\n");
				exit(0);
			}
			else if (!strcmp(word[2], "from")) {
				//do nothing
				//it is what should be
			}
			else {
				printf("Keyword 'from' is expected.\n");
				exit(0);
			}

			//Variable
			if (word[3] == NULL) {
				printf("Variable is expected.\n");
				exit(0);
			}
			else if (isKeyword(word[3])) {
				printf("%s is a keyword. Variable is expected.\n", word[3]);
				exit(0);
			}
			else if (isIntegerVariable(word[3])) {
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[3])) {
						Integers[i] -= subInt;
						break;
					}
				}
			}else if(isVariable(word[3]) && !isIntegerVariable(word[3])){
				printf("%s is not identified before!",word[3]);
				exit(0);
		}
			else {
				printf("%s is not a variable. Variable is expected.\n", word[3]);
				exit(0);
			}

			//End Of Line
			if (wordCount == 4) {
				//do nothing
				//it is what should be
			}
			else {
				printf("End of line is expected.\n");
				exit(0);
			}
		}

		//Out Keyword
		else if (!strcmp(word[0], "out")) { //if first word in line is out ,we observe the rules for out.
			if(wordCount == 2){ // out variable. => 2 word out type
				if (word[1] == NULL) {
					printf("Integer, variable or string are expected.\n");
					exit(0);

				}
				else if (isIntegerVariable(word[1])) {
					if(word[1][strlen(word[1])-1] == ','){
						 word[1][strlen(word[1])-1] = '\0';
					}
					for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
							printf("%i ",Integers[i]);
							break;
						}
					}
				}
				else if (isInteger(word[1])) {
					int tempInteger = atoi(word[1]);
					printf("%i ",tempInteger);
				}
				else if (isStringConstantParse(word[1],analyseFile)) {

				}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
						printf("%s is not identified before!",word[1]);
						exit(0);
				}else {
					printf("%s \n", word[1]);

				}
			}
			else if(wordCount ==3){ //out variable, newline. => 3 word out type.
				//Integer, Variable Or String
				if (word[1] == NULL) {
					printf("Integer, variable or string are expected.\n");
					exit(0);

				}else if (isIntegerVariable(word[1])) {
					if(word[1][strlen(word[1])-1] == ','){
						 word[1][strlen(word[1])-1] = '\0';
					}

					for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
							printf("%i ",Integers[i]);
							break;
						}
					}
				}
				else if (isInteger(word[1])) {
					printf("%s ",word[1]);
				}
				else if (isStringConstantParse(word[1],analyseFile)) {

				}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
					printf("%s is not identified before!",word[1]);
					exit(0);
			}
				else {
					printf("%s \n", word[1]);
				}

				if (word[2] == NULL) {
					printf("Keyword newline is expected \n");
					exit(0);
				}
				else if (!strcmp(word[2],"newline")) {
					printf("\n");
				}
			}else if(wordCount == 4){ //out newline, "StringConstant:", variable. => 4 word out type
				if (word[1] == NULL) {
					printf("newline , StringConstant or act is expected \n");
					exit(0);
				}
				else if (isStringConstantParse(word[1],analyseFile)) {

				}
				else if (!strcmp(word[1],"newline,")) {
					isStringConstantParse(word[2],analyseFile);
					if(isIntegerVariable(word[3])){
						for (int i = 0; i < vIndex; i++) {
							if (!strcmp(Variables[i], word[3])) {
									printf("%i ",Integers[i]);
									break;
								}
							}

				}else if (isInteger(word[3])) {
						printf("%s ",word[3]);
				}else if(isVariable(word[3]) && !isIntegerVariable(word[3])){
						printf("%s is not identified before!",word[3]);
						exit(0);
				}else{
						printf("Identifier is Expected \n");
						exit(0);
					}

					if(wordCount == 4){
					}else{
						printf("EndOfLine is expected\n");
						exit(0);
					}
					printf("\n");
				}

			}
			}


		//Loop Keyword
		else if (!strcmp(word[0], "loop")) { //if first word in line is loop ,we observe the rules for loop.
			if (word[1] == NULL) {
				printf("Integer or variable are expected.\n");
				exit(0);
			}
			else if (isKeyword(word[1])) {
				printf("%s is a keyword. Integer or variable are expected.\n", word[1]);
				exit(0);
			}
			else if (isInteger(word[1])) {

				loopInt = atoi(word[1]);
				if(loopInt < 0){ // loop counter mustn't be negative.So we checked it out.
					printf("loop counter value can not be negative!");
					exit(0);
				}
			}
			else if (isIntegerVariable(word[1])) {
				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(Variables[i], word[1])) {
						if(Integers[i]<0){
							printf("loop counter value can not be negative!");
							exit(0);
						}else{
							loopInt = Integers[i];
							strcpy(loopIntWord,word[1]);
							break;
						}

					}
				}
			}else if(isVariable(word[1]) && !isIntegerVariable(word[1])){
				printf("%s is not identified before!",word[1]);
				exit(0);
		}
			else {
				printf("%s is not a integer or variable. Integer or variable are expected.\n", word[1]);
				exit(0);
			}

			//Times Keyword
			if (word[2] == NULL) {

				printf("Keyword 'times' is expected.\n");
				exit(0);
			}
			else if (!strcmp(word[2], "times")) {
				//do nothing
				// it is what should be
			}
			else {
				printf("Keyword 'times' is expected.\n");
				exit(0);
			}
			for(int i = 0;i<loopInt;i++){
				char newCode[100] = "";
					for (int k = 3; k < wordCount; k++) {
						strcat(newCode, " ");
						strcat(newCode, word[k]);
					}
					strcat(newCode, ".");
					parse(newCode,analyseFile);
			}
		}else{
			printf("%s", word[0]);
		}
	}
}


bool isKeyword(char* str){
if(
		!strcmp(str,"int") ||
		!strcmp(str,"move") ||
		!strcmp(str,"to") ||
		!strcmp(str,"add") ||
		!strcmp(str,"sub") ||
		!strcmp(str,"from") ||
		!strcmp(str,"loop") ||
		!strcmp(str,"times") ||
		!strcmp(str,"out") ||
		!strcmp(str,"newline")
){
	return true;
}
return false;
}

char comma[100]; char k[100]; char l[100];
void isComma(char* str,FILE* analyseFile) {	// answer for =>  Does word have any comma ?
   for (int i = 0; i < strlen(str); i++) {
      if (str[i] == ',') {
    	  fprintf(analyseFile,"Seperator \n");
      }
   }
}

void isPoint(char* str,FILE* analyseFile) { // answer for =>  Does word have any point ?
	char wordPointer[20];
   for (int i = 0; i < strlen(str); i++) {

      if (str[i] == '.' && i == strlen(str)-1) {
    	  fprintf(analyseFile,"EndOfLine \n");
      }else if(str[i] == '.' && i < strlen(str)-1){
    	  substring(str,i,strlen(str),wordPointer,analyseFile);
    	  lexicalAnalyse(wordPointer,analyseFile);
      }
   }
}

bool isInteger(char* str) { // integer value => eg. 5,45,1,89 so on.
	int len = strlen(str);
	if(str[0] == '+'){
		return false;
	}
	if((str[0]=='-' && !isdigit(str[1]))){
			return false;
		}
	for (int i = 1; i < len; i++) {
			if (str[i] == '-') {
				return false;
		}
		if (!(isdigit(str[i]))) {
			return false;
		}
	}
	return true;
}

bool isIntegerVariable(char* str){ // strings that have integer value => size = 5 => so size is a integerVariable

	if(str[strlen(str)-1] == ','){
		str[strlen(str)-1] = '\0';
	}

	for(int i = 0;i < vIndex;i++){
		if(!strcmp(str,Variables[i])){
			return true;
		}
	}
	return false;
}

bool isVariable(char* str) //strings that are not keyword
{
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[0] == '_') {
					return false;
				}
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) {
			return false;
		}

		if( (str[i] == '.')|| (str[i] == ',')){
			return false;
		}
	}

	return true;
}

bool isVariable2(char* str,FILE* analyseFile) //strings that is not a keyword and not have a seperator or point at the and of the yourself
{

		if ((str >= 'a' && str <= 'z') || (str >= 'A' && str <= 'Z')) {
			return true;
		}
	return false;
}
void lexicalAnalyse(char* code,FILE* analyseFile) {
	char* tempCode[100];
	strcpy(tempCode,code);
	int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(code, "\n");

	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, "\n");

	}


	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char* word[1000];
		char* splitLine = strtok(line[i], " \r[]\"");

		while (splitLine != NULL) {
			word[wordCount++] = splitLine;

			splitLine = strtok(NULL, " \r");
		}


		if (wordCount == 0) { return; }
		bool hasPoint = false;
		bool hasComma = false;
		bool trueComment = false;
		char commentLine[100];
		char pre[100];
		char next[100];
		int j = 0;
		char* tempWordA[20];
		char tempWordA2[20];
		for(int a = 0;a<wordCount;a++){
			strcpy(tempWordA,word[a]);
			strcpy(tempWordA2,word[a]);
			for(int c=0;c<strlen(word[a]);c++){ // if the word has comma or point at the end of yourself ,
					//we remove these word[a] but we assign original word into tempWordA

					if(word[a][strlen(word[a])-1] == '.'){
						hasPoint = true;
						word[a][strlen(word[a])-1] = '\0';
					}else if(word[a][strlen(word[a])-1] == ','){
						hasComma = true;
						word[a][strlen(word[a])-1] = '\0';
					}
				}

				if(isKeyword(tempWordA)){ // Keyword control
					fprintf(analyseFile,"Keyword %s \n",word[a]);
				}else if(!strcmp(tempWordA,".")){
					fprintf(analyseFile,"EndOfLine \n");
				}else if(!strcmp(tempWordA,",")){
					fprintf(analyseFile,"Seperator \n");
				}else if(!strcmp(word[a],"[")){
					fprintf(analyseFile,"OpenBlock \n");
				}else if(!strcmp(word[a],"]")){
					fprintf(analyseFile,"CloseBlock \n");
				}else if(isVariable(tempWordA)){
					fprintf(analyseFile,"Identifier %s \n",word[a]);
				}else if(isInteger(tempWordA)){ // int constant control
					fprintf(analyseFile,"IntConstant %s \n",word[a]);
				}else if(isStringConstant(tempWordA,analyseFile)){ //string constant control
					fprintf(analyseFile,"StringConstant : %s \n",word[a]);
					isComma(tempWordA,analyseFile);
					isPoint(tempWordA,analyseFile);
				}else if(word[a][0] == '{'){ // comment line control
						for (j = a; j < wordCount; j++) {
							if (word[j][strlen(word[j])-1] == '}') {
								for(int m = 0;m < (j-a+1);m++){
									strcat(commentLine,word[m+a]);
									strcat(commentLine," ");
								}
								printf("Comment Line: %s \n", commentLine);
								trueComment = true;
								a=j;
							}
						}
						if (!trueComment) {
							printf("Comment Line is not completed.\n");
							//exit(0);
						}

				}else{	// here is going to catch invalid values eg.(non utf-8 characters,wrong length of string and integer)
					char stringVariable[20] = "";
					char temp[20];
					char temp2[20];
					char temp3[20];
					char temp4[20];

					for(int c = 0;c<strlen(tempWordA2);c++){ // if string contain point or seperator or something else,
						//this word comes here and program understand what it is without seperator
						if(tempWordA2[c] == '.'){
							fprintf(analyseFile,"EndOfLine \n");
							substring(tempWordA2,c+1,strlen(tempWordA2),temp2,analyseFile);
							lexicalAnalyse(temp2,analyseFile);
							break;
						}else if(tempWordA2[c] == ','){
							fprintf(analyseFile,"Seperator \n");
							substring(tempWordA2,c+1,strlen(tempWordA2),temp3,analyseFile);
							lexicalAnalyse(temp3,analyseFile);
							break;
						}
						else if(isVariable2(tempWordA2[c],analyseFile)){
							int j = 0;
							while(isVariable2(tempWordA2[j],analyseFile)){
								stringVariable[j] = tempWordA2[j+c];
								j++;
							}
								c = j;
								substring(tempWordA2,c,strlen(tempWordA2),temp,analyseFile);
								lexicalAnalyse(stringVariable,analyseFile);
								lexicalAnalyse(temp,analyseFile);
								break;

					}else{
						fprintf(analyseFile,"word : %s is an invalid input  \n",word[a]);
					}
				}

				}
				}
		}
	}

char* commentLine1[100]; char* a1[100]; char* b1[100];
void isCommentLine(char* str,FILE* analyseFile) { //this funtion detects comment part of code
	char* strTemp[200];
	strcpy(strTemp,str);
	bool trueComment = false;
	bool openBlock = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			openBlock = true;
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					int k = 0;
					if(k == 0){
						substring(str, i + 1, j - i - 1, commentLine1,analyseFile);
						printf("Comment Line: %s \n", commentLine1);

						substring(str, 0, i, a1,analyseFile);
						substring(str, j + 1, strlen(str), b1,analyseFile);
						strcat(a1," ");
						strcat(a1, b1);
						strcpy(str, a1);
						strcpy(strTemp,str);

						trueComment = true;

					}
					isLexicalLoop(str,analyseFile);
					isParseLoop(strTemp,analyseFile);
					k++;

				}
			}
			if (!trueComment) {
				printf("Comment Line is not completed.\n");
				exit(0);
			}
		}
	}
	if(!openBlock){
		isLexicalLoop(str,analyseFile);
		isParseLoop(strTemp,analyseFile);
	}
}

char* detectLoop[100]; char* cv[100]; char* dv[100];
void isLexicalLoop(char* str,FILE* analyseFile) {	//If there is any loop in the word , we evaluate the code according to loop (for [ ] ).Otherwise we evaluate normally the code.
	bool isOpenBlock = false;
		bool trueLoopParse = false;
		int j;
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] == '[') {
				isOpenBlock = true;
				substring(str, 0, i, cv,analyseFile);
				lexicalAnalyse(cv,analyseFile);
				fprintf(analyseFile,"OpenBlock \n");
				for (j = i; j < strlen(str); j++) {
					if (str[j] == ']') {
						substring(str, i+1 , j - i -1, detectLoop,analyseFile);
						lexicalAnalyse(detectLoop,analyseFile);
						fprintf(analyseFile,"CloseBlock \n");
						substring(str, j + 1, strlen(str), dv,analyseFile);
						isLexicalLoop(dv,analyseFile);
						trueLoopParse = true;

					}
				}

				if (!trueLoopParse) {
					printf("Loop Parsing is not completed.\n");
				}
			}
}
		if(!isOpenBlock){
			lexicalAnalyse(str,analyseFile);
				}
}

int substring(char* source, int from, int to, char* target,FILE* analyseFile){
	int length,i;
	for(length=0;source[length] != '\0';length++);
	if(from > length){
		printf("Starting index is bigger than length of word.\n");
		return 1;
	}
	if((from + to) > length){
		to = length - from;
	}
	for(i=0;i<to;i++){
		target[i] = source[from+i];
	}
	target[i] = '\0';
}

int substring(char* source, int from, int to, char* target,FILE* analyseFile);
char* detectLoop[100]; char* cv[100]; char* dv[100];
void isParseLoop(char* str,FILE* analyseFile) {
		bool trueLoopParse = false;
		int j;
		for (int i = 0; i < strlen(str); i++) {
			if (str[i] == '[') {
				for (j = i; j < strlen(str); j++) {
					if (str[j] == ']') {
						substring(str, i+1 , j - i -1, detectLoop,analyseFile);
						substring(str, 0, i, cv,analyseFile);
						substring(str, j + 1, strlen(str), dv,analyseFile);
						parse(cv,analyseFile);
						isLoop(str,analyseFile);

						parse(dv,analyseFile);
						trueLoopParse = true;

					}
				}

				if (!trueLoopParse) {
					printf("Loop Parsing is not completed.\n");
					exit(0);
				}
			}
}
		if(!trueLoopParse){
			parse(str,analyseFile);
		}
}


int main()
{
	printf("enter your input : ");
	char str[20];
	scanf("%[^\n]%*c", str); // if we want to parse input string
	int wordCount = 0;
	char *input[1000];
	char *splitCode = strtok(str, " "); // split input according to space

	while (splitCode != NULL) {
		input[wordCount++] = splitCode;
		splitCode = strtok(NULL, " ");
	}
	if(wordCount == 1){
		if(!strcmp(str,"myscript")){

			FILE* analyseFile=fopen("myscript.lx","w"); // open myscript.lx file to write process
			FILE *fp;
			long lSize;
			char* code; //code is read from myscript.ba
			char* fileName;
			char* codeTemp[1000];


			fileName = strcat(str,".ba");
			fp = fopen(fileName, "rb");
			if (!fp) perror(fileName), exit(1);

			fseek(fp, 0L, SEEK_END);
			lSize = ftell(fp);
			rewind(fp);

			code = calloc(1, lSize + 1);
			if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

			if (1 != fread(code, lSize, 1, fp)) {
				fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);
			}
			strcpy(codeTemp,code);

			isCommentLine(code,analyseFile);

		}else{
			printf("Wrong input.File could not be opened!");
		}
	}else{
		printf("Too much word.Try another input!");
	}
}
