#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <bits/stdc++.h>
#include <cstring>



using namespace std;
//Server side

void swap(int a,int b)
{
    int temp;
    temp =a;
    a = b;
    b = temp;
}
// find gcd
int gcd(int a, int b)
{
    int t;
    while (1)
    {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

double * encryption(int key[], int len, double ek, double n1)
{
    
        int i;
        double cipher_text[len+1];
        double* c_text = new double[len+1];
    
        for(i=0;i<len;i++)
        {
    	    c_text[i] = pow(key[i], ek); //encrypt the message
	}
	for(i=0;i<len;i++)
        {
	    //cout<<"Cypher Text";
	    cipher_text[i] = fmod(key[i], n1);
        }
	return c_text;
}
void decryption(double c_key[], double ek1, double dk2, double n2, int l)
{
        double  d_key[l+1]; 
        int i;
        for(i=0;i<l;i++)
        {
             d_key[i] = pow(c_key[i], dk2);
	    }
	    for(i=0;i<l;i++)
	    {
	        c_key[i] = fmod(c_key[i], n2);
            
	    }
	    //cout<< "Decrypted key: ";
	    for(i=0;i<l;i++)
	    {
    		d_key[i] = fmod(d_key[i], n2);
	    }


	    
}  

char* decryption_using_RC4(int l, int key[] , int ct[])
{
    //cout<<"\nDecrypted text: ";
    int mt[l];
    for(int i=0;i<l;i++)
    {
        mt[i] = ct[i]^key[i];
    }
    static char output[1500];
    memset(&output,0,sizeof(output));

    for(int i=0;i<l;i++)
    {
        int y = mt[i];
		
        output[i]=(char)y;
    }
    return output;
}
  
int * encrytion_using_RC4(string s, int key[])
{
    int l = s.length();
    char c_arr[l+1];
    int pt[l+1],i;
    int* ct = new int[l+1];
    strcpy(c_arr, s.c_str());
    for(i=0;i<l;i++)
    {
        char x = c_arr[i];
        pt[i] = int(x);
    }
   
    
    
    //cout<<"Encrypted text: ";
    
    for(int i=0;i<l;i++)
    {
        ct[i] = key[i]^pt[i];
    }
    for(int i=0;i<l;i++)
    {
        int y = ct[i];
    }

    return ct;
}

int main()
{
    int s[226], t[256];
    int i,t1;
    string key ="I AM THE KEY";
    int len = key.length();
    int k[len+1];
    char char_arr[len+1];
    strcpy(char_arr, key.c_str());
    for(i=0;i<len;i++)
    {
        char x = char_arr[i];
        k[i] = int(x);
    }
    cout<<endl;
    
    //2 random prime numbers for RSA
    double p = 13;
    double q = 11;
    double n = p * q; //calculate n
    double track;
    double phi = (p - 1) * (q - 1); //calculate phi
    
   
    
    //Message input
    string str;
    int len_pt = str.length();
    
    int key_arr[len_pt+1];
    
    //initializing s array 
    for(i=0;i<256;i++)
    {
        s[i] = i;
        t[i] = k[i%len];
    }

    int j = 0;
    
    //Key Scheduling
    for(i=0;i<256;i++)
    {
        j = (j + s[i] + t[i]);
        j = fmod(j,256);
        swap(s[i],s[j]);
    }
    
    //Key Stream Generation 
    i=0;
    j = 0;
    for(i=1;i<=len_pt;i++)
    {
        j = (j + s[i]);
        j = fmod(j,256);
        swap(s[i], s[j]);
        t1 = (s[i] + s[j]);
        t1 = fmod(t1,256);
        key_arr[i-1] = s[t1]; 
    }
    
    
    //public key using RSA
    //e stands for encrypt
    double e = 7;
    
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi)
    {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
     
    //private key
    //d stands for decrypt
    //choosing d such that it satisfies d*e = 1 mod phi
    double d1 = 1 / e;
    double d = fmod(d1, phi);
    int l = str.length();


    char msg[1500];
      int cipher[1500];	
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(5050);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
    sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl<<endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
  
    int bytesRead, bytesWritten = 0;
    while(1)
    {
        //receive a message from the client (listen)
        cout << "Awaiting client response..." << endl<<endl;
        memset(&msg, 0, 1500);//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
	int msglen = strlen(msg);
	
	for(i=0;i<msglen;i++)
        {	
		int y;
		y=msg[i];
		cipher[i]=y;
	}  
    
        
        
    
    key ="I AM THE KEY";
    len = key.length();
    int k_temp[len+1];
    char char_arr_temp[len+1];
    strcpy(char_arr_temp, key.c_str());
    for(i=0;i<len;i++)
    {
        char x = char_arr_temp[i];
        k_temp[i] = int(x);
    }
    cout<<endl; 
    
    //Message input
    
   
    int key_arr[msglen+1];
    
    //initializing s array 
    for(i=0;i<256;i++)
    {
        s[i] = i;
        t[i] = k[i%len];
    }

  
    
    //Key Scheduling
    for(i=0;i<256;i++)
    {
        j = (j + s[i] + t[i]);
        j = fmod(j,256);
        swap(s[i],s[j]);
    }
    
    //Key Stream Generation 
    i=0;
    j = 0;
    for(i=1;i<=msglen;i++)
    {
        j = (j + s[i]);
        j = fmod(j,256);
        swap(s[i], s[j]);
        t1 = (s[i] + s[j]);
        t1 = fmod(t1,256);
        key_arr[i-1] = s[t1]; 
    }
    
    
    //public key using RSA
    //e stands for encrypt
    
    
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi)
    {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
     
    //private key
    //d stands for decrypt
    //choosing d such that it satisfies d*e = 1 mod phi
     d1 = 1 / e;
     d = fmod(d1, phi);
	double *cipher_key = encryption(key_arr,msglen,e,n); 
        decryption(cipher_key,e,d,n,msglen);//decrypting public key using RSA
	char *msgoutput = decryption_using_RC4(msglen, key_arr , cipher);//decrypting cipher text using RC4
        if(!strcmp(msg, "exit"))
        {
            cout << "Client has quit the session" << endl<<endl;
            break;
        }
	cout<<"Encrypted Text: ";
	for(i=0;i<msglen;i++)
	{
		cout<<msg[i];
	}
        cout <<endl<< "Client: " << msgoutput << endl<<endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, 1500);
        strcpy(msg, data.c_str());
        int len_pt = strlen(msg);
        
    
    
   	key_arr[len_pt+1];
    
    //initializing s array 
    for(i=0;i<256;i++)
    {
        s[i] = i;
        t[i] = k[i%len];
    }

    int j = 0;
    
    //Key Scheduling
    for(i=0;i<256;i++)
    {
        j = (j + s[i] + t[i]);
        j = fmod(j,256);
        swap(s[i],s[j]);
    }
    
    //Key Stream Generation 
    i=0;
    j = 0;
    for(i=1;i<=len_pt;i++)
    {
        j = (j + s[i]);
        j = fmod(j,256);
        swap(s[i], s[j]);
        t1 = (s[i] + s[j]);
        t1 = fmod(t1,256);
        key_arr[i-1] = s[t1]; 
    }
    double e = 7;
    
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while (e < phi)
    {
        track = gcd(e, phi);
        if (track == 1)
            break;
        else
            e++;
    }
     
    //private key
    //d stands for decrypt
    //choosing d such that it satisfies d*e = 1 mod phi
    double d1 = 1 / e;
    double d = fmod(d1, phi);
    int l = data.length();
    int *cipher = encrytion_using_RC4(data, key_arr); // encrypting plain text using RC4
    cipher_key = encryption(key_arr,len_pt,e,n);// encrypting public key of RC4
	       
	if(data == "exit")
       	{
       	    //send to the client that server has closed the connection
	     send(newSd, (char*)&msg, strlen(msg), 0);
       	     break;
        }
	for(int i=0;i<len_pt;i++)
    	{
        	int y = cipher[i];
		msg[i]=(char)y;
    	} 
        
	//send the message to client
	bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }
    //we need to close the socket descriptors after we're all done

    close(newSd);
    close(serverSd);
    cout << "**Session Ended**" << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}

