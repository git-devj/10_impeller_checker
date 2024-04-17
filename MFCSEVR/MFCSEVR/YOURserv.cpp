#include "pch.h"
#include "YOURserv.h"
#include <iostream>
#include <fstream>
#include "UserSock.h"
#include <mysql.h>


void YOURserv::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	Accept(m_cpp);

	std::cout << "Accept---------------------- " << std::endl;
	int fileSize;

	m_cpp.Receive((char*)&fileSize, sizeof(fileSize));
	std::cout << "Accept : " << fileSize << std::endl;

	if (fileSize == 5) {
		Sleep(1000);
		RecvSearch();
	}
	else {
		char* buf = new char[fileSize];
		m_cpp.Receive(buf, fileSize);

		const char* path = "./SAVE/serv.jpg";

		std::ofstream file(path, std::ios::binary);
		if (file.is_open())
		{
			file.write(buf, fileSize);
			file.close();
		}

		delete[] buf;

		std::cout << "file data save  " << std::endl;
	}
}

void YOURserv::RecvSearch() 
{
	char strBuffer[1024];

	int nLen = m_cpp.Receive(strBuffer, sizeof(strBuffer));
	strBuffer[nLen] = 0;
	std::cout << "서버 날짜 받음 : " << strBuffer << std::endl;
	char cquery[100];
	sprintf_s(cquery, "SELECT filename, result, reason FROM result_table WHERE date = '%s'", strBuffer);
	std::cout << cquery << std::endl;


	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;

	mysql_init(&conn);
	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, (char*)NULL, 0);
	if (connect == NULL)
	{
		CString s = _T("");
		s = mysql_error(&conn);
		AfxMessageBox(s, MB_OK);
		return;
	}

	std::string message;
	std::string comma = ",";
	if (mysql_query(&conn, cquery) == 0)
	{
		sql_result = mysql_store_result(&conn);
		if (mysql_num_rows(sql_result) != 0)
		{
			MYSQL_ROW row;

			while ((row = mysql_fetch_row(sql_result)))
			{
				for (int i = 0; i < 3; i++)
				{
					message += (row[i] + comma);
				}
			}
			message.pop_back();
			std::cout << message.c_str() << std::endl;
			m_cpp.Send(message.c_str(), message.length());

			mysql_free_result(sql_result);
		}
		else
		{
			// 결과가 없는 경우
			message = "no" + comma;
			m_cpp.Send(message.c_str(), message.length());
			std::cout << message.c_str() << std::endl;
		}
	}
	else
	{
		// 쿼리 실행 오류
		message = "no" + comma;
		m_cpp.Send(message.c_str(), message.length());
		std::cout << message.c_str() << std::endl;
	}
}


