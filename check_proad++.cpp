/*
 *
 * compile: g++ -C -xc++ -Os -o check_proad++ check_proad++.cpp  -lcurl -ljsoncpp 
 * autor: Enio Ribeiro <eniobribeiro@gmail.com>
 * date: 07/2017
 *
 */
  
#include <iostream>
//#include <unistd.h>
//#include <math.h>
//#include <fstream>

#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include <string>
#include <sstream>

using namespace std;


string data;
int result = 3;//UNKNOWN
stringstream msg;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
	for (int c = 0; c<size*nmemb; c++)
	{
		data.push_back(buf[c]);
	}
	return size*nmemb;
}


int main(int argc, char* argv[]){

	int httpCode(0);


	try{
		CURL *curl;
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, "https://proad.trtsp.jus.br/proad/trt12/monitor");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
		curl_easy_perform(curl);

	
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		curl_easy_cleanup(curl);
		curl_global_cleanup();


		if (httpCode == 200){

			Json::Value jsonData;
        		Json::Reader jsonReader;

		
			if (jsonReader.parse(data, jsonData))
        		{

				if(argc == 1){
					
					if(jsonData["status"] == "ATIVO"){
						result = 0;
						msg << "OK - Serviço Ativo [versão: " << jsonData["versao"].asString() << "]";
					}
				
				}else{		
				
		
					for(int i = 0; i < jsonData["recursos"].size(); i++){

						if(jsonData["recursos"][i]["nome"] == argv[1]){
							string status =  jsonData["recursos"][i]["status"].asString();
							
							if(status == "ATIVO"){
								result =  0 ;//OK
								msg << "OK - Serviço Ativo [latência: " << jsonData["recursos"][i]["latencia"].asString() << "]";
							}else{
								result = 2;//CRITICAL
								msg << "ERROR - O serviço não foi localizado";
							}
						}
					}
				}
        		}else{
				result = 2;//CRITICAL
				msg << "ERROR - O serviço não foi localizado";
			}

		}else{
			
		}

	}catch(int e){
		result = 2;//CRITICAL	
		msg << "ERROR - O serviço não foi localizado";
	}


	cout <<  msg.str();

	

	return result; 
}
