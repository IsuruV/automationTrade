#pragma once
#include <string>



namespace HighFrequencyTrading {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		//variables for moving average calculations
		int numberOfPrices;
		double priceTotal;
		// used to initate stream of bid, ask, last price data
		bool butStart = false, butStart2 = false;
		// list to hold price history
		System::Collections::Generic::List<double>^ lstPriceHistory;
		// list of 20 period moving average
		System::Collections::Generic::List<double>^ lstMovingAverage20;
		//list of 100 period moving average
		System::Collections::Generic::List<double>^ lstMovingAverage100;

	public:
		bool submit = false; 
		bool filled = false;
	private: System::Windows::Forms::TextBox^  tbValue;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::ListBox^  lb100MA;
	private: System::Windows::Forms::ListBox^  lb20MA;
	private: System::Windows::Forms::ListBox^  lbLastPrice;
	private: System::Windows::Forms::Button^  Start;
	public:
		long orderid;
		// submit moving average : golden cross; cross of death
		void MovingAverageSubmit()
		{
			for (int count; count < lstMovingAverage100->Count; count++)
			{
				// golden cross
				while (lstMovingAverage20[count] < lstMovingAverage100[count])
				{
					if (lstMovingAverage20[count] == lstMovingAverage100[count])
					{
						double subPrice;
						subPrice = lstMovingAverage100[count];
						newOrderID();
						submit_function(orderid, subPrice);


					}

				}
				while (lstMovingAverage20[count] > lstMovingAverage100[count])
				{

				}
			}

		}
		////
		// submit limit order function
		void submit_function(long orderid, double lmprice)
		{
			
				// Create a new ContractInfo object
				TWSLib::IContract^ ContractInfo = axTws1->createContract();
				//TWSLib::_DTwsEvents_accountSummaryEventHandler^ reqAccountSummary;

				// Create a new Order object
				TWSLib::IOrder^ OrderInfo = axTws1->createOrder();
				// Fill the ContractInfo object with the necessary information 
				// Contract identifier (set to 0 if unknown)
				ContractInfo->conId = 0;
				// The ticker symbol for the order
				ContractInfo->symbol = this->tbTicker->Text;
				// Type of instrument: Stock=STK,Option=OPT,Future=FUT, etc.
				ContractInfo->secType = "STK";
				// The Options or Futures expiration data in the format YYYYMM
				ContractInfo->expiry = "";
				// The Options Strike Price 
				ContractInfo->strike = 0;
				// The Options "PUT" or "CALL" 
				ContractInfo->right = "";
				// The contract multiplier for Futures or Options
				ContractInfo->multiplier = "";
				// The destination of order or request. "SMART" =IB order router
				ContractInfo->exchange = this->tbExchange->Text;
				// The primary exchange where the instrument trades. 
				// NYSE, NASDAQ, AMEX, BATS, ARCA, PHLX etc.
				ContractInfo->primaryExchange = this->tbPrimaryEx->Text;
				// The currency of the exchange USD or GBP or CAD or EUR, etc.
				ContractInfo->currency = "USD";
				// Fill in the Order information:
				// orderId is a *Unique* order identifier
				OrderInfo->orderId = orderid;
				// Action will be BUY, SELL or SSHORT
				OrderInfo->action = this->tbAction->Text;
				// totalQuantity is the total number of shares (or contracts)
				OrderInfo->totalQuantity = long::Parse(this->tbQuantity->Text);
				// orderType is one of MKT, LMT, STP, etc.
				OrderInfo->orderType = this->tbOrderType->Text;
				// lmtPrice is the limit price for a limit order
				OrderInfo->lmtPrice = lmprice;
				// timeInForce is either DAY, GTC, IOC, GTD etc.
				OrderInfo->timeInForce = "DAY";
				// There are about 40 other properties for different order types...
				// Submit the order
				this->axTws1->placeOrderEx(long::Parse(this->tbOrderID->Text),
					ContractInfo, OrderInfo);
				
				//input into lbLog
				String ^ y;
				y = "Submitted: " + orderid + "," + tbAction->Text + "," + tbQuantity->Text + "," + tbTicker->Text
					+ "," + tbPrimaryEx->Text + "," + lmprice + ".";

				this->lbLog->Items->Add(y);
			

		}//end

		void cancelOrder(long int orderid){
			// Cancel the order. Takes one parameter:
			//  - OrderID
			this->axTws1->cancelOrder(orderid);
			
			String ^ y;
			y = "CANCELED: " + orderid + "," + tbAction->Text + "," + tbQuantity->Text + "," + tbTicker->Text
				+ "," + tbPrimaryEx->Text + "," + tbLimitPrice->Text + ".";

			this->lbLog->Items->Add(y);
		}
		
		
		//request new order id function
		void newOrderID(){
			 	this->axTws1->reqIds(1);
				orderid = long::Parse(this->tbOrderID->Text);
		}





	private: AxTWSLib::AxTws^  axTws2;
	private: System::Windows::Forms::Button^  Stop;
	public:

		System::Collections::Generic::List<double>^ bidPriceHistory;
		System::Collections::Generic::List<long int>^ orderIdHistory;
		Form1(void)
		{
			InitializeComponent();
			//holds bid prices
			bidPriceHistory = gcnew System::Collections::Generic::List<double>();
		    //holds order ids
			orderIdHistory = gcnew System::Collections::Generic::List<long int>();
			// holds price history to calculate Moving averages
			lstPriceHistory = gcnew System::Collections::Generic::List<double>();
			// 20 period moving average
			lstMovingAverage20 = gcnew System::Collections::Generic::List<double>();
			// 100 period moving average
			lstMovingAverage100 = gcnew System::Collections::Generic::List<double>();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListBox^  lbAskPrice;
	protected:
	private: System::Windows::Forms::ListBox^  lbBidPrice;
	private: System::Windows::Forms::Label^  Bid;
	private: System::Windows::Forms::Label^  label1;
	private: AxTWSLib::AxTws^  axTws1;
	private: System::Windows::Forms::Button^  btnStart;
	private: System::Windows::Forms::Button^  btnStop;
	private: System::Windows::Forms::TextBox^  tbTicker;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  btnConnect;
	private: System::Windows::Forms::TextBox^  tbOrderID;
	private: System::Windows::Forms::TextBox^  tbAction;
	private: System::Windows::Forms::TextBox^  tbQuantity;

	private: System::Windows::Forms::TextBox^  tbAvgPrice;





	private: System::Windows::Forms::TextBox^  tbRemaining;

	private: System::Windows::Forms::TextBox^  tbFilled;

	private: System::Windows::Forms::TextBox^  tbStatus;

	private: System::Windows::Forms::TextBox^  tbLimitPrice;

	private: System::Windows::Forms::TextBox^  tbOrderType;


	private: System::Windows::Forms::TextBox^  tbPrimaryEx;

	private: System::Windows::Forms::TextBox^  tbExchange;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;

	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  newBid;
	private: System::Windows::Forms::Button^  btnSubmit;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::ListBox^  lbErrors;
	private: System::Windows::Forms::ListBox^  lbLog;
	private: System::Windows::Forms::TextBox^  tbAccountValue;
	private: System::Windows::Forms::Button^  btnAutoBid;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->lbAskPrice = (gcnew System::Windows::Forms::ListBox());
			this->lbBidPrice = (gcnew System::Windows::Forms::ListBox());
			this->Bid = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->axTws1 = (gcnew AxTWSLib::AxTws());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btnStop = (gcnew System::Windows::Forms::Button());
			this->tbTicker = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnConnect = (gcnew System::Windows::Forms::Button());
			this->tbOrderID = (gcnew System::Windows::Forms::TextBox());
			this->tbAction = (gcnew System::Windows::Forms::TextBox());
			this->tbQuantity = (gcnew System::Windows::Forms::TextBox());
			this->tbAvgPrice = (gcnew System::Windows::Forms::TextBox());
			this->tbRemaining = (gcnew System::Windows::Forms::TextBox());
			this->tbFilled = (gcnew System::Windows::Forms::TextBox());
			this->tbStatus = (gcnew System::Windows::Forms::TextBox());
			this->tbLimitPrice = (gcnew System::Windows::Forms::TextBox());
			this->tbOrderType = (gcnew System::Windows::Forms::TextBox());
			this->tbPrimaryEx = (gcnew System::Windows::Forms::TextBox());
			this->tbExchange = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->newBid = (gcnew System::Windows::Forms::TextBox());
			this->btnSubmit = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->lbErrors = (gcnew System::Windows::Forms::ListBox());
			this->lbLog = (gcnew System::Windows::Forms::ListBox());
			this->tbAccountValue = (gcnew System::Windows::Forms::TextBox());
			this->btnAutoBid = (gcnew System::Windows::Forms::Button());
			this->axTws2 = (gcnew AxTWSLib::AxTws());
			this->Stop = (gcnew System::Windows::Forms::Button());
			this->tbValue = (gcnew System::Windows::Forms::TextBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->lbLastPrice = (gcnew System::Windows::Forms::ListBox());
			this->lb20MA = (gcnew System::Windows::Forms::ListBox());
			this->lb100MA = (gcnew System::Windows::Forms::ListBox());
			this->Start = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axTws1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axTws2))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->SuspendLayout();
			// 
			// lbAskPrice
			// 
			this->lbAskPrice->FormattingEnabled = true;
			this->lbAskPrice->Location = System::Drawing::Point(414, 67);
			this->lbAskPrice->Margin = System::Windows::Forms::Padding(2);
			this->lbAskPrice->Name = L"lbAskPrice";
			this->lbAskPrice->Size = System::Drawing::Size(82, 212);
			this->lbAskPrice->TabIndex = 0;
			// 
			// lbBidPrice
			// 
			this->lbBidPrice->FormattingEnabled = true;
			this->lbBidPrice->Location = System::Drawing::Point(298, 67);
			this->lbBidPrice->Margin = System::Windows::Forms::Padding(2);
			this->lbBidPrice->Name = L"lbBidPrice";
			this->lbBidPrice->Size = System::Drawing::Size(85, 212);
			this->lbBidPrice->TabIndex = 1;
			// 
			// Bid
			// 
			this->Bid->AutoSize = true;
			this->Bid->Location = System::Drawing::Point(333, 48);
			this->Bid->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->Bid->Name = L"Bid";
			this->Bid->Size = System::Drawing::Size(22, 13);
			this->Bid->TabIndex = 2;
			this->Bid->Text = L"Bid";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(441, 48);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(25, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Ask";
			// 
			// axTws1
			// 
			this->axTws1->Enabled = true;
			this->axTws1->Location = System::Drawing::Point(916, 531);
			this->axTws1->Margin = System::Windows::Forms::Padding(2);
			this->axTws1->Name = L"axTws1";
			this->axTws1->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"axTws1.OcxState")));
			this->axTws1->Size = System::Drawing::Size(189, 50);
			this->axTws1->TabIndex = 4;
			this->axTws1->tickPrice += gcnew AxTWSLib::_DTwsEvents_tickPriceEventHandler(this, &Form1::axTws1_tickPrice);
			this->axTws1->nextValidId += gcnew AxTWSLib::_DTwsEvents_nextValidIdEventHandler(this, &Form1::axTws1_nextValidId);
			this->axTws1->errMsg += gcnew AxTWSLib::_DTwsEvents_errMsgEventHandler(this, &Form1::axTws1_errMsg);
			this->axTws1->orderStatus += gcnew AxTWSLib::_DTwsEvents_orderStatusEventHandler(this, &Form1::axTws1_orderStatus);
			this->axTws1->accountSummary += gcnew AxTWSLib::_DTwsEvents_accountSummaryEventHandler(this, &Form1::axTws1_accountSummary);
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(317, 2);
			this->btnStart->Margin = System::Windows::Forms::Padding(2);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(50, 22);
			this->btnStart->TabIndex = 5;
			this->btnStart->Text = L"Start";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &Form1::btnStart_Click);
			// 
			// btnStop
			// 
			this->btnStop->Location = System::Drawing::Point(431, 3);
			this->btnStop->Margin = System::Windows::Forms::Padding(2);
			this->btnStop->Name = L"btnStop";
			this->btnStop->Size = System::Drawing::Size(50, 21);
			this->btnStop->TabIndex = 6;
			this->btnStop->Text = L"Stop";
			this->btnStop->UseVisualStyleBackColor = true;
			this->btnStop->Click += gcnew System::EventHandler(this, &Form1::btnStop_Click);
			// 
			// tbTicker
			// 
			this->tbTicker->Location = System::Drawing::Point(50, 168);
			this->tbTicker->Margin = System::Windows::Forms::Padding(2);
			this->tbTicker->Name = L"tbTicker";
			this->tbTicker->Size = System::Drawing::Size(68, 20);
			this->tbTicker->TabIndex = 7;
			this->tbTicker->Text = L"MSFT";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(2, 171);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 13);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Symbol";
			// 
			// btnConnect
			// 
			this->btnConnect->Location = System::Drawing::Point(8, 11);
			this->btnConnect->Margin = System::Windows::Forms::Padding(2);
			this->btnConnect->Name = L"btnConnect";
			this->btnConnect->Size = System::Drawing::Size(87, 34);
			this->btnConnect->TabIndex = 9;
			this->btnConnect->Text = L"Connect";
			this->btnConnect->UseVisualStyleBackColor = true;
			this->btnConnect->Click += gcnew System::EventHandler(this, &Form1::btnConnect_Click);
			// 
			// tbOrderID
			// 
			this->tbOrderID->Location = System::Drawing::Point(50, 45);
			this->tbOrderID->Margin = System::Windows::Forms::Padding(2);
			this->tbOrderID->Name = L"tbOrderID";
			this->tbOrderID->Size = System::Drawing::Size(68, 20);
			this->tbOrderID->TabIndex = 10;
			this->tbOrderID->DoubleClick += gcnew System::EventHandler(this, &Form1::tbOrderID_DoubleClick);
			// 
			// tbAction
			// 
			this->tbAction->Location = System::Drawing::Point(50, 79);
			this->tbAction->Margin = System::Windows::Forms::Padding(2);
			this->tbAction->Name = L"tbAction";
			this->tbAction->Size = System::Drawing::Size(68, 20);
			this->tbAction->TabIndex = 11;
			this->tbAction->Text = L"BUY";
			// 
			// tbQuantity
			// 
			this->tbQuantity->Location = System::Drawing::Point(50, 124);
			this->tbQuantity->Margin = System::Windows::Forms::Padding(2);
			this->tbQuantity->Name = L"tbQuantity";
			this->tbQuantity->Size = System::Drawing::Size(68, 20);
			this->tbQuantity->TabIndex = 12;
			this->tbQuantity->Text = L"100";
			// 
			// tbAvgPrice
			// 
			this->tbAvgPrice->Location = System::Drawing::Point(198, 255);
			this->tbAvgPrice->Margin = System::Windows::Forms::Padding(2);
			this->tbAvgPrice->Name = L"tbAvgPrice";
			this->tbAvgPrice->Size = System::Drawing::Size(68, 20);
			this->tbAvgPrice->TabIndex = 14;
			// 
			// tbRemaining
			// 
			this->tbRemaining->Location = System::Drawing::Point(198, 208);
			this->tbRemaining->Margin = System::Windows::Forms::Padding(2);
			this->tbRemaining->Name = L"tbRemaining";
			this->tbRemaining->Size = System::Drawing::Size(68, 20);
			this->tbRemaining->TabIndex = 15;
			// 
			// tbFilled
			// 
			this->tbFilled->Location = System::Drawing::Point(198, 161);
			this->tbFilled->Margin = System::Windows::Forms::Padding(2);
			this->tbFilled->Name = L"tbFilled";
			this->tbFilled->Size = System::Drawing::Size(68, 20);
			this->tbFilled->TabIndex = 16;
			// 
			// tbStatus
			// 
			this->tbStatus->Location = System::Drawing::Point(198, 124);
			this->tbStatus->Margin = System::Windows::Forms::Padding(2);
			this->tbStatus->Name = L"tbStatus";
			this->tbStatus->Size = System::Drawing::Size(68, 20);
			this->tbStatus->TabIndex = 17;
			// 
			// tbLimitPrice
			// 
			this->tbLimitPrice->Location = System::Drawing::Point(198, 79);
			this->tbLimitPrice->Margin = System::Windows::Forms::Padding(2);
			this->tbLimitPrice->Name = L"tbLimitPrice";
			this->tbLimitPrice->Size = System::Drawing::Size(68, 20);
			this->tbLimitPrice->TabIndex = 18;
			// 
			// tbOrderType
			// 
			this->tbOrderType->Location = System::Drawing::Point(198, 38);
			this->tbOrderType->Margin = System::Windows::Forms::Padding(2);
			this->tbOrderType->Name = L"tbOrderType";
			this->tbOrderType->Size = System::Drawing::Size(68, 20);
			this->tbOrderType->TabIndex = 19;
			this->tbOrderType->Text = L"LMT";
			// 
			// tbPrimaryEx
			// 
			this->tbPrimaryEx->Location = System::Drawing::Point(50, 255);
			this->tbPrimaryEx->Margin = System::Windows::Forms::Padding(2);
			this->tbPrimaryEx->Name = L"tbPrimaryEx";
			this->tbPrimaryEx->Size = System::Drawing::Size(68, 20);
			this->tbPrimaryEx->TabIndex = 20;
			this->tbPrimaryEx->Text = L"NASDAQ";
			this->tbPrimaryEx->TextChanged += gcnew System::EventHandler(this, &Form1::tbPrimaryEx_TextChanged);
			// 
			// tbExchange
			// 
			this->tbExchange->Location = System::Drawing::Point(50, 211);
			this->tbExchange->Margin = System::Windows::Forms::Padding(2);
			this->tbExchange->Name = L"tbExchange";
			this->tbExchange->Size = System::Drawing::Size(68, 20);
			this->tbExchange->TabIndex = 21;
			this->tbExchange->Text = L"SMART";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1, 45);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(47, 13);
			this->label3->TabIndex = 22;
			this->label3->Text = L"Order ID";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(149, 168);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(31, 13);
			this->label4->TabIndex = 23;
			this->label4->Text = L"Filled";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(143, 124);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(37, 13);
			this->label5->TabIndex = 24;
			this->label5->Text = L"Status";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(137, 41);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(60, 13);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Order Type";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(-3, 211);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(55, 13);
			this->label7->TabIndex = 26;
			this->label7->Text = L"Exchange";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(1, 126);
			this->label9->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(46, 13);
			this->label9->TabIndex = 28;
			this->label9->Text = L"Quantity";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(139, 82);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(55, 13);
			this->label10->TabIndex = 29;
			this->label10->Text = L"Limit Price";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(2, 259);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(56, 13);
			this->label11->TabIndex = 30;
			this->label11->Text = L"Pimary Ex.";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(1, 82);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(37, 13);
			this->label12->TabIndex = 31;
			this->label12->Text = L"Action";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(136, 259);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(56, 13);
			this->label13->TabIndex = 32;
			this->label13->Text = L"Avg. Price";
			this->label13->Click += gcnew System::EventHandler(this, &Form1::label13_Click);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(136, 211);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(57, 13);
			this->label14->TabIndex = 33;
			this->label14->Text = L"Remaining";
			// 
			// newBid
			// 
			this->newBid->Location = System::Drawing::Point(298, 67);
			this->newBid->Margin = System::Windows::Forms::Padding(2);
			this->newBid->Name = L"newBid";
			this->newBid->Size = System::Drawing::Size(85, 20);
			this->newBid->TabIndex = 34;
			// 
			// btnSubmit
			// 
			this->btnSubmit->Location = System::Drawing::Point(317, 283);
			this->btnSubmit->Margin = System::Windows::Forms::Padding(2);
			this->btnSubmit->Name = L"btnSubmit";
			this->btnSubmit->Size = System::Drawing::Size(50, 31);
			this->btnSubmit->TabIndex = 35;
			this->btnSubmit->Text = L"Submit";
			this->btnSubmit->UseVisualStyleBackColor = true;
			this->btnSubmit->Click += gcnew System::EventHandler(this, &Form1::btnSubmit_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(431, 283);
			this->btnCancel->Margin = System::Windows::Forms::Padding(2);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(50, 31);
			this->btnCancel->TabIndex = 36;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &Form1::btnCancel_Click);
			// 
			// lbErrors
			// 
			this->lbErrors->FormattingEnabled = true;
			this->lbErrors->Location = System::Drawing::Point(50, 337);
			this->lbErrors->Margin = System::Windows::Forms::Padding(2);
			this->lbErrors->Name = L"lbErrors";
			this->lbErrors->Size = System::Drawing::Size(244, 56);
			this->lbErrors->TabIndex = 37;
			// 
			// lbLog
			// 
			this->lbLog->FormattingEnabled = true;
			this->lbLog->Location = System::Drawing::Point(516, 76);
			this->lbLog->Margin = System::Windows::Forms::Padding(2);
			this->lbLog->Name = L"lbLog";
			this->lbLog->Size = System::Drawing::Size(344, 108);
			this->lbLog->TabIndex = 38;
			// 
			// tbAccountValue
			// 
			this->tbAccountValue->Location = System::Drawing::Point(617, 34);
			this->tbAccountValue->Margin = System::Windows::Forms::Padding(2);
			this->tbAccountValue->Name = L"tbAccountValue";
			this->tbAccountValue->Size = System::Drawing::Size(119, 20);
			this->tbAccountValue->TabIndex = 39;
			// 
			// btnAutoBid
			// 
			this->btnAutoBid->Location = System::Drawing::Point(306, 337);
			this->btnAutoBid->Margin = System::Windows::Forms::Padding(2);
			this->btnAutoBid->Name = L"btnAutoBid";
			this->btnAutoBid->Size = System::Drawing::Size(77, 32);
			this->btnAutoBid->TabIndex = 40;
			this->btnAutoBid->Text = L"Auto Bid/Cancel";
			this->btnAutoBid->UseVisualStyleBackColor = true;
			this->btnAutoBid->Click += gcnew System::EventHandler(this, &Form1::btnAutoBid_Click);
			// 
			// axTws2
			// 
			this->axTws2->Enabled = true;
			this->axTws2->Location = System::Drawing::Point(710, 337);
			this->axTws2->Name = L"axTws2";
			this->axTws2->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"axTws2.OcxState")));
			this->axTws2->Size = System::Drawing::Size(168, 50);
			this->axTws2->TabIndex = 41;
			this->axTws2->accountSummary += gcnew AxTWSLib::_DTwsEvents_accountSummaryEventHandler(this, &Form1::axTws2_accountSummary);
			// 
			// Stop
			// 
			this->Stop->Location = System::Drawing::Point(414, 337);
			this->Stop->Name = L"Stop";
			this->Stop->Size = System::Drawing::Size(79, 32);
			this->Stop->TabIndex = 42;
			this->Stop->Text = L"Stop";
			this->Stop->UseVisualStyleBackColor = true;
			this->Stop->Click += gcnew System::EventHandler(this, &Form1::Stop_Click);
			// 
			// tbValue
			// 
			this->tbValue->Location = System::Drawing::Point(617, 4);
			this->tbValue->Name = L"tbValue";
			this->tbValue->Size = System::Drawing::Size(100, 20);
			this->tbValue->TabIndex = 43;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Location = System::Drawing::Point(4, 50);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(935, 448);
			this->tabControl1->TabIndex = 44;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->Start);
			this->tabPage2->Controls->Add(this->lb100MA);
			this->tabPage2->Controls->Add(this->lb20MA);
			this->tabPage2->Controls->Add(this->lbLastPrice);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(927, 422);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->label13);
			this->tabPage1->Controls->Add(this->label14);
			this->tabPage1->Controls->Add(this->tbFilled);
			this->tabPage1->Controls->Add(this->btnCancel);
			this->tabPage1->Controls->Add(this->tbRemaining);
			this->tabPage1->Controls->Add(this->axTws2);
			this->tabPage1->Controls->Add(this->btnSubmit);
			this->tabPage1->Controls->Add(this->Stop);
			this->tabPage1->Controls->Add(this->btnStop);
			this->tabPage1->Controls->Add(this->btnAutoBid);
			this->tabPage1->Controls->Add(this->btnStart);
			this->tabPage1->Controls->Add(this->tbValue);
			this->tabPage1->Controls->Add(this->label3);
			this->tabPage1->Controls->Add(this->label12);
			this->tabPage1->Controls->Add(this->label9);
			this->tabPage1->Controls->Add(this->tbAccountValue);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->lbLog);
			this->tabPage1->Controls->Add(this->label7);
			this->tabPage1->Controls->Add(this->lbErrors);
			this->tabPage1->Controls->Add(this->label11);
			this->tabPage1->Controls->Add(this->newBid);
			this->tabPage1->Controls->Add(this->tbOrderID);
			this->tabPage1->Controls->Add(this->tbAction);
			this->tabPage1->Controls->Add(this->tbQuantity);
			this->tabPage1->Controls->Add(this->tbTicker);
			this->tabPage1->Controls->Add(this->tbExchange);
			this->tabPage1->Controls->Add(this->label4);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->label5);
			this->tabPage1->Controls->Add(this->lbAskPrice);
			this->tabPage1->Controls->Add(this->tbAvgPrice);
			this->tabPage1->Controls->Add(this->tbStatus);
			this->tabPage1->Controls->Add(this->Bid);
			this->tabPage1->Controls->Add(this->label10);
			this->tabPage1->Controls->Add(this->lbBidPrice);
			this->tabPage1->Controls->Add(this->tbPrimaryEx);
			this->tabPage1->Controls->Add(this->tbLimitPrice);
			this->tabPage1->Controls->Add(this->label6);
			this->tabPage1->Controls->Add(this->tbOrderType);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(927, 422);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// lbLastPrice
			// 
			this->lbLastPrice->FormattingEnabled = true;
			this->lbLastPrice->Location = System::Drawing::Point(116, 67);
			this->lbLastPrice->Name = L"lbLastPrice";
			this->lbLastPrice->Size = System::Drawing::Size(78, 290);
			this->lbLastPrice->TabIndex = 0;
			// 
			// lb20MA
			// 
			this->lb20MA->FormattingEnabled = true;
			this->lb20MA->Location = System::Drawing::Point(270, 67);
			this->lb20MA->Name = L"lb20MA";
			this->lb20MA->Size = System::Drawing::Size(83, 290);
			this->lb20MA->TabIndex = 1;
			// 
			// lb100MA
			// 
			this->lb100MA->FormattingEnabled = true;
			this->lb100MA->Location = System::Drawing::Point(437, 67);
			this->lb100MA->Name = L"lb100MA";
			this->lb100MA->Size = System::Drawing::Size(86, 290);
			this->lb100MA->TabIndex = 2;
			// 
			// Start
			// 
			this->Start->Location = System::Drawing::Point(40, 23);
			this->Start->Name = L"Start";
			this->Start->Size = System::Drawing::Size(75, 23);
			this->Start->TabIndex = 3;
			this->Start->Text = L"Start";
			this->Start->UseVisualStyleBackColor = true;
			this->Start->Click += gcnew System::EventHandler(this, &Form1::Start_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(951, 510);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->axTws1);
			this->Controls->Add(this->btnConnect);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axTws1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axTws2))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void axTws1_tickPrice(System::Object^  sender, AxTWSLib::_DTwsEvents_tickPriceEvent^  e)
	{
				 // When this Event Handler is activated, price changes 
				 // will appear in the Events objects named "e" 
				 // Properties of "e" include:
				 // e->id()        The identifier that was set during the call to reqMktData
				 // e->price()     The latest Price
				 // e->tickType()  The tick type 1=bid,2=ask, 4=last, 6=high, 7=low, 9=close 
				 // e->canAutoExecute()   A flag 1= the order can be automatically executed
				 // If this is a last price change, then display it 
				 if (butStart2 == true){
					 if (e->tickType == 1) {
						 // Add the latest price to the list box
						 this->lbBidPrice->Items->Add(e->price);
						 this->newBid->Text = Convert::ToString(e->price);
						 //
						 this->bidPriceHistory->Add(e->price - 0.02);
						 this->orderIdHistory->Add(e->id);


						 if (submit == true){
							 //cancel order
							 cancelOrder(orderid);

							 //submit a new order
							 newOrderID();

							 submit_function(orderid, e->price - 0.02);
						 }



						 /*





						if (bidPriceHistory->Count == 3){
						bidPriceHistory->RemoveAt(0);
						}


						if (submit == true){
						if (filled == false){
						//cancel old order
						cancelOrder(orderid);
						//submit new order
						newOrderID();
						long orderid2;
						orderid2 = long::Parse(this->tbOrderID->Text);
						submit_function(orderid2, bidPriceHistory[1]);
						}

						}
						*/


					 }//end of if bid price
				 }//
				 if (butStart2 == true){
					 if (e->tickType == 2) {
						 // Add the latest price to the list box
						 this->lbAskPrice->Items->Add(e->price);

					 } // end if tick type is a ask price
				 }////

				 if (butStart == true){
					 if (e->tickType == 4)
					 {

						 // add the last price to the price history list
						 lstPriceHistory->Add(e->price);
						 // populate last price listbox
						 lbLastPrice->Items->Add(e->price);

						 //used to calculate 20 period Moving average
						 double sum20MA = 0, avg20MA = 0;
						 double sum100MA = 0, avg100MA = 0;

						 if (lstPriceHistory->Count < 10)
						 {
							 lb20MA->Items->Add("0");
						 }
						 else if (lstPriceHistory->Count > 10)
						 {
							 lstPriceHistory->RemoveAt(0);

							 for (int count = 0; count < 10; count++)
							 {
								 sum20MA += lstPriceHistory[count];
							 }
							 avg20MA = sum20MA / 10;
							 lb20MA->Items->Add(avg20MA);
							 // populate 20 MA list
							 //*gives error check to see if one of the options below work when markets open
							 
						//	 lstMovingAverage20->Add(Convert::ToDouble(lb20MA->Items->ToString()));
							
							 lstMovingAverage20->Add(double::Parse(lb20MA->Items->ToString()));
							
							
						 }
						 if (lstPriceHistory->Count < 20)
						 {
							 lb100MA->Items->Add(0);
						 }
						 else if (lstPriceHistory->Count > 20)
						 {
							 lstPriceHistory->RemoveAt(0);

							 for (int count = 0; count < 20; count++)
							 {
								 sum100MA += lstPriceHistory[count];
							 }
							 avg100MA = sum100MA / 20;
							 lb100MA->Items->Add(avg100MA);
							 // populate 100 MA list
							 lstMovingAverage100->Add(Convert::ToDouble(lb100MA));
						 }





					 }

				 } // if



	}


	////////////////////////////////
	

	//////////////////////////////////
	private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 butStart2 = true;
				 // Create a new contract object
				 TWSLib::IContract^ ContractInfo = axTws1->createContract();
				 // Create an Empty TagValue List object. For API Version 9.71
				 TWSLib::ITagValueList^ mktDataOptions = axTws1->createTagValueList();
				 // Now fill the ContractInfo object with the necessary information 
				 // Contract identifier
				 ContractInfo->conId = 0;
				 // Stock symbol
				 // ContractInfo->symbol = "MSFT";  
				 ContractInfo->symbol = tbTicker->Text;
				 // Type of instrument: Stock=STK,Option=OPT,Future=FUT, etc.
				 ContractInfo->secType = "STK";
				 // The Options or Futures expiration data in the format YYYYMM
				 ContractInfo->expiry = "";
				 // The Options Strike Price 
				 ContractInfo->strike = 0;
				 // The Options "PUT" or "CALL" 
				 ContractInfo->right = "";
				 // The contract multiplier for Futures or Options
				 ContractInfo->multiplier = "";
				 // The destination of order or request. "SMART" =IB order router
				 ContractInfo->exchange = "SMART";
				 // The primary exchange where the instrument trades. 
				 // NYSE, NASDAQ, AMEX, BATS, ARCA, PHLX etc.
				 ContractInfo->primaryExchange = "NASDAQ";
				 // The currency of the exchange USD or GBP or CAD or EUR, etc.
				 ContractInfo->currency = "USD";
				 // The local exchange symbol of the underlying asset.
				 // ContractInfo->localSymbol = "MSFT"; 
				 ContractInfo->localSymbol = tbTicker->Text;

				 //   TickerID - use a unique number for each request
				 //   ContractInfo - the Contract object containing the financial instrument
				 //   genericTicks - Can be used to get different tick types (use "" for now)
				 //   Snapshot (0 for streaming, 1 to get a single data point)
				 //   mktDataOptions - an empty TagValueList used for API version 9.71
				 axTws1->reqMktDataEx(0, ContractInfo, "", 0, mktDataOptions);
				 //   Note: If you receive an error asking for only 4 parameters, remove the
				 //   fifth parameter - mktDataOptions
	}
	private: System::Void btnConnect_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // Connect to the IB Data Server
				 // The First parameter is the IP address of the server. Leave this blank
				 // The Second parameter is the network port. Used the default of 7496
				 // The third parameter is the client application identifier
				 this->axTws1->connect("", 7496, 0);

				 // initalize moving average variables
				 numberOfPrices = 0;
				 priceTotal = 0;
	}
	private: System::Void btnStop_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // Cancel the request for market data
				 // Parameters are:
				 //  - OrderID  This should be the identifier used in the original call to reqMktData
				 this->axTws1->cancelMktData(0);
				 // Disconnect from the IB Data Server
				 this->axTws1->disconnect();

	}
			 /*
			 private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
			 */

			 /*private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }*/
	private: System::Void btnSubmit_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // Create a new ContractInfo object
				 TWSLib::IContract^ ContractInfo = axTws1->createContract();
				 //TWSLib::_DTwsEvents_accountSummaryEventHandler^ reqAccountSummary;

				 // Create a new Order object
				 TWSLib::IOrder^ OrderInfo = axTws1->createOrder();
				 // Fill the ContractInfo object with the necessary information 
				 // Contract identifier (set to 0 if unknown)
				 ContractInfo->conId = 0;
				 // The ticker symbol for the order
				 ContractInfo->symbol = this->tbTicker->Text;
				 // Type of instrument: Stock=STK,Option=OPT,Future=FUT, etc.
				 ContractInfo->secType = "STK";
				 // The Options or Futures expiration data in the format YYYYMM
				 ContractInfo->expiry = "";
				 // The Options Strike Price 
				 ContractInfo->strike = 0;
				 // The Options "PUT" or "CALL" 
				 ContractInfo->right = "";
				 // The contract multiplier for Futures or Options
				 ContractInfo->multiplier = "";
				 // The destination of order or request. "SMART" =IB order router
				 ContractInfo->exchange = this->tbExchange->Text;
				 // The primary exchange where the instrument trades. 
				 // NYSE, NASDAQ, AMEX, BATS, ARCA, PHLX etc.
				 ContractInfo->primaryExchange = this->tbPrimaryEx->Text;
				 // The currency of the exchange USD or GBP or CAD or EUR, etc.
				 ContractInfo->currency = "USD";
				 // Fill in the Order information:
				 // orderId is a *Unique* order identifier
				 OrderInfo->orderId = long::Parse(this->tbOrderID->Text);
				 // Action will be BUY, SELL or SSHORT
				 OrderInfo->action = this->tbAction->Text;
				 // totalQuantity is the total number of shares (or contracts)
				 OrderInfo->totalQuantity = long::Parse(this->tbQuantity->Text);
				 // orderType is one of MKT, LMT, STP, etc.
				 OrderInfo->orderType = this->tbOrderType->Text;
				 // lmtPrice is the limit price for a limit order
				 OrderInfo->lmtPrice = double::Parse(this->tbLimitPrice->Text);
				 // timeInForce is either DAY, GTC, IOC, GTD etc.
				 OrderInfo->timeInForce = "DAY";
				 // There are about 40 other properties for different order types...
				 // Submit the order
				 this->axTws1->placeOrderEx(long::Parse(this->tbOrderID->Text),
					 ContractInfo, OrderInfo);

				 //input into lbLog
				 String ^ y;
				 y = "Submitted: " + tbOrderID->Text + "," + tbAction->Text + "," + tbQuantity->Text + "," + tbTicker->Text
					 + "," + tbPrimaryEx->Text + "," + tbLimitPrice->Text + ".";

				 this->lbLog->Items->Add(y);

	}//end of submit button

	public: System::Void axTws1_orderStatus(System::Object^  sender, AxTWSLib::_DTwsEvents_orderStatusEvent^  e)
	{
				 // Get the status of an order. 
				 // Responds to every change in order status.
				 // Status in in event object e:
				 // e->id              The OrderID (long)
				 // e->status          The status of the order (string)
				 // e->filled          The number of shares/contracts filled in the so far (long)
				 // e->remaining       The number of shares/contracts remaining in the order (long)
				 // e->avgFillPrice    The average fill price (double)
				 // e->permId          The permenent order Id (long)
				 // e->lastFilledPrice The price of the last partial fill (double)
				 // e->clientId        The Id of the client who placed the order (long)
				 this->tbStatus->Text = e->status;
				
				 if (e->status == "Filled" && e->id == orderIdHistory[0])
				 {
					 filled = true;
				 }
				 this->tbFilled->Text = Convert::ToString(e->filled);
				 this->tbRemaining->Text = Convert::ToString(e->remaining);
				 this->tbAvgPrice->Text = Convert::ToString(e->avgFillPrice);

	}
	private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // Cancel the order. Takes one parameter:
				 //  - OrderID
				 this->axTws1->cancelOrder(long::Parse(this->tbOrderID->Text));

				 String ^ y;
				 y = "CANCELED: " + tbOrderID->Text + "," + tbAction->Text + "," + tbQuantity->Text + "," + tbTicker->Text
					 + "," + tbPrimaryEx->Text + "," + tbLimitPrice->Text + ".";

				 this->lbLog->Items->Add(y);

	}
	private: System::Void axTws1_errMsg(System::Object^  sender, AxTWSLib::_DTwsEvents_errMsgEvent^  e)
	{
				 lbErrors->Items->Add(e->errorMsg);
	}
	private: System::Void tbOrderID_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
				 //new unique id
				 this->axTws1->reqIds(1);
	}

	private: System::Void axTws1_nextValidId(System::Object^  sender, AxTWSLib::_DTwsEvents_nextValidIdEvent^  e)
	{
				 this->tbOrderID->Text = Convert::ToString(e->id);
				 orderid = (e->id);


	}
	private: System::Void axTws1_accountSummary(System::Object^  sender, AxTWSLib::_DTwsEvents_accountSummaryEvent^  e)
	{
				 this->tbAccountValue->Text = Convert::ToString(e->tag);


	}

			 //auto submit 
	private: System::Void btnAutoBid_Click(System::Object^  sender, System::EventArgs^  e) {

				 //whatever the price is, submit the first order
				 newOrderID();
				 orderid = long::Parse(this->tbOrderID->Text);
				 submit_function(orderid, bidPriceHistory[0]);
				 this->orderIdHistory->Add(orderid);

				 submit = true;

				 // requests account info once button is pushed. 
				 this->axTws2->reqAccountSummary(355666, "All", "TotalCashValue");

				 /*
				 newOrderID();

				 long orderid;
				 orderid = long::Parse(this->tbOrderID->Text);

				 submit_function(orderid, bidPriceHistory[0]);
				 */
	}//end of btnAutoBid_CLick
	private: System::Void Stop_Click(System::Object^  sender, System::EventArgs^  e)
	{
				 // stops stream of ask/bid
				 butStart2 = false;
				 // stops stream of last price
				 submit = false; 

	}
			 //event handler for pulling accounting info
private: System::Void axTws2_accountSummary(System::Object^  sender, AxTWSLib::_DTwsEvents_accountSummaryEvent^  e) 
{
			// this->tbFilled->Text = Convert::ToString(e->filled);
			// this->tbValue->Text = Convert::ToString(e->tag=TotalCashValue);

			 if (e->tag == "AccountType"){
			 
				 this->tbValue->Text = Convert::ToString(e->value);
			}
			 if (e->tag == "AvailableFunds"){

				 this->tbValue->Text = Convert::ToString(e->value);
			 }

}

		 
private: System::Void tbPrimaryEx_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void label13_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void Start_Click(System::Object^  sender, System::EventArgs^  e) 
{
			 butStart = true;
			 
				 // Create a new contract object
				 TWSLib::IContract^ ContractInfo = axTws1->createContract();
				 // Create an Empty TagValue List object. For API Version 9.71
				 TWSLib::ITagValueList^ mktDataOptions = axTws1->createTagValueList();
				 // Now fill the ContractInfo object with the necessary information 
				 // Contract identifier
				 ContractInfo->conId = 0;
				 // Stock symbol
				 // ContractInfo->symbol = "MSFT";  
				 ContractInfo->symbol = tbTicker->Text;
				 // Type of instrument: Stock=STK,Option=OPT,Future=FUT, etc.
				 ContractInfo->secType = "STK";
				 // The Options or Futures expiration data in the format YYYYMM
				 ContractInfo->expiry = "";
				 // The Options Strike Price 
				 ContractInfo->strike = 0;
				 // The Options "PUT" or "CALL" 
				 ContractInfo->right = "";
				 // The contract multiplier for Futures or Options
				 ContractInfo->multiplier = "";
				 // The destination of order or request. "SMART" =IB order router
				 ContractInfo->exchange = "SMART";
				 // The primary exchange where the instrument trades. 
				 // NYSE, NASDAQ, AMEX, BATS, ARCA, PHLX etc.
				 ContractInfo->primaryExchange = "NASDAQ";
				 // The currency of the exchange USD or GBP or CAD or EUR, etc.
				 ContractInfo->currency = "USD";
				 // The local exchange symbol of the underlying asset.
				 // ContractInfo->localSymbol = "MSFT"; 
				 ContractInfo->localSymbol = tbTicker->Text;

				 //   TickerID - use a unique number for each request
				 //   ContractInfo - the Contract object containing the financial instrument
				 //   genericTicks - Can be used to get different tick types (use "" for now)
				 //   Snapshot (0 for streaming, 1 to get a single data point)
				 //   mktDataOptions - an empty TagValueList used for API version 9.71
				 axTws1->reqMktDataEx(0, ContractInfo, "", 0, mktDataOptions);
				 //   Note: If you receive an error asking for only 4 parameters, remove the
				 //   fifth parameter - mktDataOptions
			 
				 MovingAverageSubmit(); 
}
};
}

