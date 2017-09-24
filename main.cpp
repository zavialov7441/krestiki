#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#define SIZE_MSG 1024

using namespace std;
const int size=3;
char buf[SIZE_MSG];    // буфер для приема
int mas[size][size] = { {0, 0, 0,}, {0, 0, 0,}, {0, 0, 0,} };
bool first=1;
int i,j;

void field()
{
for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++)
        cout << " " << mas[i][j];
    cout << endl; }
}

bool winner1(){
if ((mas[0][0]==1 && mas[1][1]==1 && mas[2][2]==1) || (mas[2][0]==1 && mas[1][1]==1 && mas[0][2]==1) || (mas[0][1]==1 && mas[1][1]==1 && mas[2][1]==1) || (mas[1][0]==1 && mas[1][1]==1 && mas[1][2]==1))
{
cout<<"Player 1 win";
return true;
}
if ((mas[0][0]==1 && mas[1][0]==1 && mas[2][0]==1) || (mas[0][0]==1 && mas[0][1]==1 && mas[0][2]==1) || (mas[0][2]==1 && mas[1][2]==1 && mas[2][2]==1) || (mas[2][0]==1 && mas[2][1]==1 && mas[2][2]==1))
{
cout<<"Player 1 win";
return true;
}
}

bool winner2(){
if ((mas[0][0]==2 && mas[1][1]==2 && mas[2][2]==2) || (mas[2][0]==2 && mas[1][1]==2 && mas[0][2]==2) || (mas[0][1]==2 && mas[1][1]==2 && mas[2][1]==2) || (mas[1][0]==2 && mas[1][1]==2 && mas[1][2]==2))
{
cout<<"Player 2 win";
return true;
}
if ((mas[0][0]==2 && mas[1][0]==2 && mas[2][0]==2) || (mas[0][0]==2 && mas[0][1]==2 && mas[0][2]==2) || (mas[0][2]==2 && mas[1][2]==2 && mas[2][2]==2) || (mas[2][0]==2 && mas[2][1]==2 && mas[2][2]==2))
{
cout<<"Player 2 win";
return true;
}
}

int main(){
    int flag = FALSE;
    cout<<"Client=0,Server=1"<< endl;
    cin>>flag;
    if(flag == TRUE){
	cout << "Server started!" << endl;
	field();
		struct sockaddr_in addr; // структура с адресом

		WSADATA wsa;            // Структура с информацией о WinSock
		SOCKET sock, listener;  // дескрипторы сокетов
		// Инициализация WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// создаем сокет для входных подключений
		if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}

		// Указываем параметры сервера
		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			exit(2);
		}

		listen(listener, 1); // очередь входных подключений

		while (1){
			sock = accept(listener, NULL, NULL); // принимаем входные подключение и создаем отделный сокет для каждого нового подключившегося клиента
			if (sock < 0) {
				perror("accept");
				exit(3);
			}
			else{
				closesocket(listener);
			}
			while (1){
				recv(sock, buf, SIZE_MSG, 0);//чтение данных из сокета
				//memset(buf, 0, SIZE_MSG);
				if(winner1() || winner2())
                {
                    return 0;
				}
				if(first){
                    first=0;
                    cout<<"Player1 your turn:"<<endl;
                    cin>>i;
                    cin>>j;
                    buf[0]=i;
                    buf[1]=j;
                    mas[i][j]=1;
                    field();
                    send(sock, buf, SIZE_MSG, 0);
				}
				else{
                cout<<"Player2 turn:"<<endl;
                i=buf[0];
                j=buf[1];
                mas[i][j]=2;
                field();
                memset(buf, 0, SIZE_MSG);
                cout<<"Player1 your turn:"<<endl;
                cin>>i;
                cin>>j;
                buf[0]=i;
                buf[1]=j;
                mas[i][j]=1;
                field();
				send(sock, buf, SIZE_MSG, 0);
			}
			}
			cout << "Player left the game" << endl;
			closesocket(sock); // закрываем сокет

		}
	return 0;
    }//server
    else{
        cout << "Client started!" << endl;
        field();
		struct sockaddr_in addr; // структура с адресом

		WSADATA wsa;        // Структура с информацией о WinSock
		SOCKET sock;        // дескриптор сокета

		// Инициализация WinSock
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			printf("Failed. Error Code : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// создаем сокет для входных подключений
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			printf("Could not create socket : %d", WSAGetLastError());
			perror("socket");
			exit(1);
		}
		// Указываем параметры сервера
		addr.sin_family = AF_INET; // домены Internet
		addr.sin_port = htons(3425); // или любой другой порт...
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		// установка соединения с сервером
		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("connect");
			exit(2);
		}
        send(sock, "Player2 ready!", SIZE_MSG, 0);
		while (1){
			recv(sock, buf, SIZE_MSG, 0);
                if(winner1() || winner2())
                {
                return 0;
                }
                cout<<"Player1 turn:"<<endl;
                i=buf[0];
                j=buf[1];
                mas[i][j]=1;
                field();
                memset(buf, 0, SIZE_MSG);
                cout<<"Player2 your turn:"<<endl;
                cin>>i;
                cin>>j;
                buf[0]=i;
                buf[1]=j;
                mas[i][j]=2;
                field();
			send(sock, buf, SIZE_MSG, 0);
            memset(buf, 0, SIZE_MSG);
		}
		closesocket(sock); // закрываем сокет
	return 0;
    } // client
}

/*
void winner(){
if ((mas[0][0]==1 && mas[1][1]==1 && mas[2][2]==1) || (mas[2][0]==1 && mas[1][1]==1 && mas[0][2]==1) || (mas[0][1]==1 && mas[1][1]==1 && mas[2][1]==1) || (mas[1][0]==1 && mas[1][1]==1 && mas[1][2]==1))
{
cout<<"Player 1 win";
send(sock, "You lose", SIZE_MSG, 0);
return 0;

}
if ((mas[0][0]==1 && mas[1][0]==1 && mas[2][0]==1) || (mas[0][0]==1 && mas[0][1]==1 && mas[0][2]==1) || (mas[0][2]==1 && mas[1][2]==1 && mas[2][2]==1) || (mas[2][0]==1 && mas[2][1]==1 && mas[2][2]==1))
{
cout<<"Player 1 win";
send(sock, "You lose", SIZE_MSG, 0);
return 0;
}
if ((mas[0][0]==2 && mas[1][1]==2 && mas[2][2]==2) || (mas[2][0]==2 && mas[1][1]==2 && mas[0][2]==2) || (mas[0][1]==2 && mas[1][1]==2 && mas[2][1]==2) || (mas[1][0]==2 && mas[1][1]==2 && mas[1][2]==2))
{
cout<<"Player 2 win";
}
if ((mas[0][0]==2 && mas[1][0]==2 && mas[2][0]==2) || (mas[0][0]==2 && mas[0][1]==2 && mas[0][2]==2) || (mas[0][2]==2 && mas[1][2]==2 && mas[2][2]==2) || (mas[2][0]==2 && mas[2][1]==2 && mas[2][2]==2))
{
cout<<"Player 2 win";
}
}

void player1()
{
    int i,j;
    cout<<"Player 1 your turn:\n";
    cin>>i;
    cin>>j;
    mas[i][j]=1;
    buf[0]=i;
    buf[1]=j;
    field();
}

void player2()
{
    int i,j;
    cin>>i;
    cin>>j;
    mas[i][j]=2;
    field();
}



*/

