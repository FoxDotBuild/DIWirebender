//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.TabControl.hpp>

#include <stdio.h>
#include <FMX.Objects.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TTabControl *TabControl1;
	TTabItem *Setup;
	TTabItem *Bend;
	TLabel *Label1;
	TEdit *FnameEdit;
	TGroupBox *JogGroupBox;
	TGroupBox *ZjogGroupBox;
	TButton *ZCCWButton;
	TButton *ZCWButton;
	TGroupBox *BenderJogGroupBox;
	TButton *CCWButton;
	TButton *CWButton;
	TGroupBox *FeedJogGroupBox;
	TButton *RetractButton;
	TButton *FeedButton;
	TButton *OpenButton;
	TEdit *StatusEdit;
	TGroupBox *PinGroupBox;
	TButton *UpButton;
	TButton *DownButton;
	TGroupBox *HomeGroupBox;
	TButton *SetHomeButton;
	TButton *GoHomeButton;
	TButton *BendButton;
	TButton *ClearButton;
	TPanel *Panel1;
	TEdit *XEdit;
	TEdit *YEdit;
	TLabel *Label2;
	TLabel *Label3;
	TPaintBox *PaintBox1;
	TEdit *LengthEdit;
	TEdit *AngleEdit;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall SetHomeButtonClick(TObject *Sender);
	void __fastcall OpenButtonClick(TObject *Sender);
	void __fastcall RetractButtonClick(TObject *Sender);
	void __fastcall FeedButtonClick(TObject *Sender);
	void __fastcall CCWButtonClick(TObject *Sender);
	void __fastcall CWButtonClick(TObject *Sender);
	void __fastcall ZCCWButtonClick(TObject *Sender);
	void __fastcall ZCWButtonClick(TObject *Sender);
	void __fastcall UpButtonClick(TObject *Sender);
	void __fastcall DownButtonClick(TObject *Sender);
	void __fastcall GoHomeButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall PaintBox1Paint(TObject *Sender, TCanvas *Canvas);
	void __fastcall PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall ClearButtonClick(TObject *Sender);
	void __fastcall BendButtonClick(TObject *Sender);

private:	// User declarations
	int PointCount;
    TPoint Points[ 64 ];
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	FILE* fp;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
