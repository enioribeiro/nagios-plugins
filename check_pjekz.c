/*
 * gcc -o check_pjekz check_pjekz.c -lcurl
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <regex.h>
#include <curl/curl.h>

struct string {
	char *ptr;
	size_t len;
};

void init_string(struct string *s) {
	s->len = 0;
	s->ptr = malloc(s->len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

size_t writeCallback(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

struct string s;


void getContent(char* url){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		res = curl_easy_perform(curl);
	}

	curl_global_cleanup();


}

void getVersion(char* source, char* search){

	char * regexString;


	if(strcmp(search,"versao") == 0){
		regexString = "modulo-versao[^>]+>([^>]+)*</h3>";
	}else{
		regexString = "modulo-nome[^>]+>([^>]+)*</h3>";
	}

	size_t maxMatches = 1;
	size_t maxGroups = 2;

	regex_t regexCompiled;
	regmatch_t groupArray[maxGroups];
	unsigned int m;
	char * cursor;

	if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
	{
		printf("Could not compile regular expression.\n");
		return ;
	};
	m = 0;
	cursor = source;
	for (m = 0; m < maxMatches; m ++)
	{

		if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
			break;  // No more matches


		unsigned int g = 0;
		unsigned int offset = 0;

		if (groupArray[1].rm_so == (size_t)-1)
			break;  // No more groups

		if (g == 0)
			offset = groupArray[1].rm_eo;

		char cursorCopy[strlen(cursor) + 1];
		strcpy(cursorCopy, cursor);
		cursorCopy[groupArray[1].rm_eo] = 0;
		printf("OK - %s\n", cursorCopy + groupArray[1].rm_so);
		cursor += offset;
	}

	regfree(&regexCompiled);

}
int main(int argc, char* argv[]){

	if(argc <= 2 ){
		printf("Argumentos Inválidos!\n Usar: check_pjekz [ Host/IP ] [ nome|versao ] \n");
		exit(0);
	}	

	
	regex_t re;
	init_string(&s);


	if (regcomp(&re, "modulo", REG_EXTENDED) != 0)
	{
		fprintf(stderr, "Failed to compile regex '%s'\n", "modulo-versao");
		return EXIT_FAILURE;
	}


	char *url[5];
	url[0] = ":8080/pje-comum-api/versao.html";
	url[1] = ":8080/pje-integracao-api/versao.html";
	url[2] = ":8080/pje-seguranca/versao.html";
	url[4] = NULL;

	int i = 0;
	char* u = (char *)malloc(sizeof(char));

	while(url[i]){
		strcpy(u, argv[1]);
		strcat(u, url[i]);


		getContent(u);
		
		
		if(regexec(&re, s.ptr, 0, NULL, 0) == 0){

			if(argc > 1 ){
				
				if(argv[2] != NULL){
					getVersion(s.ptr, argv[2]);	
					free(s.ptr);
					exit(0);
				}
				
			}

		}

		i++;
	}
	free(u);
	free(s.ptr);
	return 0;
}
