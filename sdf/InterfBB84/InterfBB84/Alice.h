#pragma once
#include <windows.h>
#include<thread>


namespace InterfBB84 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class Alice : public System::Windows::Forms::Form
	{

	private:
		void doSome(int j)
		{

			for (size_t i = 0; i < 3; i++)
			{
				Sleep(1000);
				//std::this_thread::sleep_for(1000);
				textBox1->Text = "peido" + i;
			}
		}

	public:
		Alice(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			std::thread thread1 = thread(doSome, 0);
			//new Thread(doSome).Start();
			//thread1 = std::thread(doSome);
			
			
			
			//textBox1->Text = "peidoLAS";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Alice()
		{
			if (components)
			{
				delete components;
			}
		}

	
	private: System::Windows::Forms::Label^  label1;
	protected:
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  bttnProtocol;
	private: System::Windows::Forms::Button^  bttnTCP;
	private: System::Windows::Forms::Button^  bttnStart;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::Label^  labelRefresh;
	private: System::Windows::Forms::Label^  labelMs;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::Label^  labelDoubleClick;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::Label^  labelNoClick;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Label^  labelFinalKey;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->bttnProtocol = (gcnew System::Windows::Forms::Button());
			this->bttnTCP = (gcnew System::Windows::Forms::Button());
			this->bttnStart = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->labelRefresh = (gcnew System::Windows::Forms::Label());
			this->labelMs = (gcnew System::Windows::Forms::Label());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->labelDoubleClick = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->labelNoClick = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->labelFinalKey = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->label1->Location = System::Drawing::Point(86, 167);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(187, 29);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Received Qubits:";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(90, 54);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(628, 22);
			this->textBox1->TabIndex = 1;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(291, 167);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(124, 29);
			this->textBox2->TabIndex = 2;
			// 
			// bttnProtocol
			// 
			this->bttnProtocol->Location = System::Drawing::Point(945, 314);
			this->bttnProtocol->Name = L"bttnProtocol";
			this->bttnProtocol->Size = System::Drawing::Size(117, 40);
			this->bttnProtocol->TabIndex = 3;
			this->bttnProtocol->Text = L"Protocol Conf.";
			this->bttnProtocol->UseVisualStyleBackColor = true;
			this->bttnProtocol->Click += gcnew System::EventHandler(this, &Alice::bttnProtocol_Click);
			// 
			// bttnTCP
			// 
			this->bttnTCP->Location = System::Drawing::Point(945, 360);
			this->bttnTCP->Name = L"bttnTCP";
			this->bttnTCP->Size = System::Drawing::Size(117, 41);
			this->bttnTCP->TabIndex = 4;
			this->bttnTCP->Text = L"TCP Conf.";
			this->bttnTCP->UseVisualStyleBackColor = true;
			// 
			// bttnStart
			// 
			this->bttnStart->Location = System::Drawing::Point(894, 407);
			this->bttnStart->Name = L"bttnStart";
			this->bttnStart->Size = System::Drawing::Size(168, 44);
			this->bttnStart->TabIndex = 5;
			this->bttnStart->Text = L"Start Simulation";
			this->bttnStart->UseVisualStyleBackColor = true;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(291, 238);
			this->textBox3->Multiline = true;
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(124, 29);
			this->textBox3->TabIndex = 7;
			// 
			// labelRefresh
			// 
			this->labelRefresh->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->labelRefresh->Location = System::Drawing::Point(86, 238);
			this->labelRefresh->Name = L"labelRefresh";
			this->labelRefresh->Size = System::Drawing::Size(159, 29);
			this->labelRefresh->TabIndex = 6;
			this->labelRefresh->Text = L"Refresh Rate:";
			this->labelRefresh->Click += gcnew System::EventHandler(this, &Alice::labelRefresh_Click);
			// 
			// labelMs
			// 
			this->labelMs->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->labelMs->Location = System::Drawing::Point(421, 238);
			this->labelMs->Name = L"labelMs";
			this->labelMs->Size = System::Drawing::Size(42, 29);
			this->labelMs->TabIndex = 8;
			this->labelMs->Text = L"ms";
			this->labelMs->Click += gcnew System::EventHandler(this, &Alice::labelMs_Click);
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(291, 367);
			this->textBox4->Multiline = true;
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(124, 29);
			this->textBox4->TabIndex = 12;
			// 
			// labelDoubleClick
			// 
			this->labelDoubleClick->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->labelDoubleClick->Location = System::Drawing::Point(86, 367);
			this->labelDoubleClick->Name = L"labelDoubleClick";
			this->labelDoubleClick->Size = System::Drawing::Size(199, 34);
			this->labelDoubleClick->TabIndex = 11;
			this->labelDoubleClick->Text = L"Double Click Rate:";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(291, 314);
			this->textBox5->Multiline = true;
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(124, 29);
			this->textBox5->TabIndex = 10;
			// 
			// labelNoClick
			// 
			this->labelNoClick->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->labelNoClick->Location = System::Drawing::Point(86, 314);
			this->labelNoClick->Name = L"labelNoClick";
			this->labelNoClick->Size = System::Drawing::Size(159, 29);
			this->labelNoClick->TabIndex = 9;
			this->labelNoClick->Text = L"No Click Rate:";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(291, 416);
			this->textBox6->Multiline = true;
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(124, 29);
			this->textBox6->TabIndex = 14;
			// 
			// labelFinalKey
			// 
			this->labelFinalKey->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->labelFinalKey->Location = System::Drawing::Point(86, 416);
			this->labelFinalKey->Name = L"labelFinalKey";
			this->labelFinalKey->Size = System::Drawing::Size(159, 29);
			this->labelFinalKey->TabIndex = 13;
			this->labelFinalKey->Text = L"Final Key Size:";
			// 
			// Alice
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1110, 488);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->labelFinalKey);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->labelDoubleClick);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->labelNoClick);
			this->Controls->Add(this->labelMs);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->labelRefresh);
			this->Controls->Add(this->bttnStart);
			this->Controls->Add(this->bttnTCP);
			this->Controls->Add(this->bttnProtocol);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Name = L"Alice";
			this->Text = L"ALICE";
			this->Load += gcnew System::EventHandler(this, &Alice::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void labelRefresh_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void bttnProtocol_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void labelMs_Click(System::Object^  sender, System::EventArgs^  e) {
}
};
}
