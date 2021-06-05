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

//Client side

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
	for(i=0;i<l;i++)
        {
	    	d_key[i] = fmod(d_key[i], n2);
	}
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
  

char* decryption_using_RC4(int l, int key[] , int ct[])
{
    //cout<<"\nDecrypted text: ";
    int mt[l];
    for(int i=0;i<l;i++)
    {
        mt[i] = ct[i]^key[i];
    }
    static char output[1500];
    memset(&output, 0, sizeof(output));
    for(int i=0;i<l;i++)
    {
        int y = mt[i];
        output[i]=(char)y;
    }
    return output;
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
	        cipher_text[i] = fmod(key[i], n1);
	}
	    
        //cout<< "Encrypted key:  ";
	return c_text;
}
  
 
int main()
{
   
    
  
    
    //we need 2 things: ip address and port number, in that order

    char msg[1500]; 
    int cipher[1500];
    
    //setup a socket and connection tools 
 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendSockAddr.sin_port = htons(5050);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl; 
    }
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    
    while(1)
    {
        cout << ">";
        string data;
        getline(cin, data);//message in data
        int len_pt = data.length();    
        memset(&msg, 0, sizeof(msg));
        strcpy(msg, data.c_str());
   
    int msglen = strlen(msg);
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
    int key_arr[msglen+1];
    
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
    //choosing d such that it satisfies d*e = 1 mod phi
    double d1 = 1 / e;
    double d = fmod(d1, phi);
    int l = strlen(msg);
    

    
    int *cipher = encrytion_using_RC4(data, key_arr); // encrypting plain text using RC4
    double *cipher_key = encryption(key_arr,l,e,n);// encrypting public key of RC4
        
    if(data == "exit")
    {
        send(clientSd, (char*)&msg, strlen(msg), 0);
        break;
    }
    for(int i=0;i<msglen;i++)
    {
       	int y = cipher[i];
	msg[i]=(char)y;
    } 
    bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);
    cout << "Awaiting server response..." << endl<<endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);     
    msglen = strlen(msg);
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
    
   
     key_arr[msglen+1];
    
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
    //choosing d such that it satisfies d*e = 1 mod phi
     d1 = 1 / e;
     d = fmod(d1, phi);
    
    
     int temp_len = strlen(msg);
     cipher_key = encryption(key_arr,temp_len,e,n);
         
     decryption(cipher_key,e,d,n,temp_len);//decrypting public key using RSA
     char *msgoutput = decryption_using_RC4(temp_len, key_arr , cipher);//decrypting cipher text using RC4
     if(!strcmp(msg, "exit"))
     {
         cout << "Server has quit the session" << endl<<endl;
         break;
     }
      cout<<"Encrypted Text: ";
     for(i=0;i<temp_len;i++)
     {
        cout<<msg[i];
     }
     cout <<endl<< "Server: " << msgoutput << endl<<endl;       
   }
 
   close(clientSd);
   cout << "**Session Ended**" << endl;
   cout << "Connection closed" << endl;
   return 0;    
}

