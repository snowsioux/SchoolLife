#pragma once
#include "DxLib.h"
#include "Header.h"
//�e�X�g

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�|�[�g�p�ϐ�
	int SendPort, RecvPort;//send����recv�󂯂�

	//����M�f�[�^�����p
	char StrBuf[256] = { "NULL" };

	//Window�̐ݒ�
	ChangeWindowMode(TRUE);//window���[�h
	SetGraphMode(640, 480, 32);//window�T�C�Y640*480 32bit
	SetAlwaysRunFlag(TRUE);//�o�b�N�O���E���h�ł����s�ł���悤�ɂ���
	SetDoubleStartValidFlag(TRUE);//���d�N���̋���

	//������
	if (DxLib_Init() == -1)return -1;//�G���[���o����I��

	//���[�h�̃Z���N�g
	int mode = SelectMode();

	//window�̖��O�ύX
	if (mode == 0)
	{
		SetWindowText("�X�^�[�g��");
		SendPort = 41;
		RecvPort = 40;
	}
	else
	{
		SetWindowText("�҂��󂯑�");
		SendPort = 40;
		RecvPort = 41;
	}

	//�`��̐ݒ�
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�̓ǂݍ���
	int image = LoadGraph("image\\sample.png");

	//�摜�̈ʒu���
	float x, y;

	//�ʒu��������
	x = 0.0f;
	y = 0.0f;

	//����M�p�f�[�^
	char name[15] = "name";
	Data* data = new Data(x, y, name);

	//IP�̐ݒ�
	IPDATA IP = IP_set();

	//�ʐM�p�\�P�b�g�n���h��
	int NetUDPHandle = MakeUDPSocket(SendPort);//�|�[�g�̐ݒ�

	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)//ESC�L�[�ŏI��
	{

		if (ProcessMessage() == -1)break;

		if (mode == 0)
		{
			if (CheckHitKey(KEY_INPUT_UP))y -= 4.0f;
			if (CheckHitKey(KEY_INPUT_DOWN))y += 4.0f;
			if (CheckHitKey(KEY_INPUT_LEFT))x -= 4.0f;
			if (CheckHitKey(KEY_INPUT_RIGHT))x += 4.0f;

			//�f�[�^��ϊ�
			//sprintf_s(StrBuf, "%f", x);//�������ϊ�

			//����M�f�[�^�̍X�V
			data->pos.x = x;
			data->pos.y = y;

			//�f�[�^�̑��M
			NetWorkSendUDP(NetUDPHandle, IP, RecvPort, data, 256);
		}

		if (mode == 1)
		{
			//�f�[�^�̎�M
			if (CheckNetWorkRecvUDP(NetUDPHandle) == TRUE)
			{
				NetWorkRecvUDP(NetUDPHandle, NULL, NULL, StrBuf, 256, FALSE);
				//��M�f�[�^��ϊ�
				memcpy_s(data, sizeof(Data), StrBuf, sizeof(Data));
				x = data->pos.x;
				y = data->pos.y;
				//x = atof(StrBuf);//�󂯎�����f�[�^��float�ɕϊ�
				//DrawFormatString(0, 16, GetColor(255, 0, 0), "%s", StrBuf);
			}
		}
		//�`��̃N���A
		ClearDrawScreen();

		//�`�揈��
		DrawGraphF(x, y, image, TRUE);

		//�o�b�N�o�b�t�@�Ɛ؂�ւ�
		ScreenFlip();

	}

	//�����L�[���͂�����܂őҋ@
	WaitKey();


	delete data;//����M�f�[�^�̍폜
	//�n���h���̍폜
	DeleteUDPSocket(NetUDPHandle);
	//dx���C�u�����I������
	DxLib_End();

	return 0;

}