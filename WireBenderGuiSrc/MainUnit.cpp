//---------------------------------------------------------------------------

#include <fmx.h>
//#include "String.h"
#include <WinAPI.devices.hpp>

#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

 // Commands have the following form:
 //
 // Fnnn  - Feed nnn millimeters
 // Rnnn  - Retract nnn millimeters
 // Bnnn  - Bend nnn degrees. nnn may be negative.
 // Mnnn  - Move the bender axis nnn degrees, but don't return like bend does
 // Hx    - Home. If x = 'S', set as home. If x = 'G' go to home
 // Znnn  - Rotate Z axis nnn degrees. nnn may be negative.
 // Sx    - Solenoid state. x=0 => Solenoid off, x=1 => Solenoid on
 //
 // All commands are ended by a carraige return and/or linefeed

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	// Initialize the array of points around which to bend
	// The first point is the end of the extruder
	PointCount = 1;
	Points[ 0 ].X = PaintBox1->Width / 2;
	Points[ 0 ].Y = PaintBox1->Height - 21;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetHomeButtonClick(TObject *Sender)
{
	char obuf[] = { 'H', 'S', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}

}

//---------------------------------------------------------------------------
void __fastcall TForm1::OpenButtonClick(TObject *Sender)
{
	AnsiString FileName;
	char fname[ 512 ];

	StatusEdit->Text = "";

	// get the filename from the edit control
	FileName = FnameEdit->Text;
	strcpy( fname, FileName.c_str() );

	fp = fopen( fname, "wb+" );

	if( fp == NULL )
	{
		StatusEdit->Text = "Cannnot open " + AnsiString( fname );
	}
	else
	{
		StatusEdit->Text = "Successfully Opened " + AnsiString( fname );
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RetractButtonClick(TObject *Sender)
{
	char obuf[] = { 'R', '1','\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
 		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FeedButtonClick(TObject *Sender)
{
	char obuf[] = { 'F', '1','\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CCWButtonClick(TObject *Sender)
{
	char obuf[] = { 'M', '-', '1', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
 }
//---------------------------------------------------------------------------
void __fastcall TForm1::CWButtonClick(TObject *Sender)
{
	char obuf[] = { 'M', '1', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ZCCWButtonClick(TObject *Sender)
{
	char obuf[] = { 'Z', '-', '1', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZCWButtonClick(TObject *Sender)
{
	char obuf[] = { 'Z', '1', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpButtonClick(TObject *Sender)
{
	char obuf[] = { 'S', '0', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DownButtonClick(TObject *Sender)
{
	char obuf[] = { 'S', '1', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GoHomeButtonClick(TObject *Sender)
{
	char obuf[] = { 'H', 'G', '\n', '\0' };

	StatusEdit->Text = "";

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	}
	else
	{
		if( fwrite( obuf, 1, strlen( obuf ), fp )  != strlen( obuf ) )
			StatusEdit->Text = "Error Writing File";
		fflush( fp );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( fp != NULL )
        fclose( fp );

}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1Paint(TObject *Sender, TCanvas *Canvas)
{
	PaintBox1->Canvas->Stroke->Kind = TBrushKind::Solid;
	PaintBox1->Canvas->Stroke->Color = claGrey;
	PaintBox1->Canvas->Stroke->Thickness = 2;

	// Draw the wire extrude head
	float Center = PaintBox1->Width / 2;
	int Bott = PaintBox1->Height - 1 ;
	TPointF p1 = TPointF( Center, Bott );	  // Sets the center of the arc
	TPointF p2 = TPointF( 20, 20 );  // sets the radius of the arc
	Canvas->DrawArc( p1, p2, 180, 180, 1.0 );  // draw the arc on the canvas

	// Draw the initial point on the extruder tip
	PaintBox1->Canvas->Stroke->Color = claBlue;
	PaintBox1->Canvas->Fill->Color = claBlue;
	TRectF Rect( Center - 2, Bott - 22, Center + 2, Bott - 18 );
	Canvas->FillEllipse( Rect, 1.0 );

    // Draw the user shape
	PaintBox1->Canvas->Stroke->Color = claBlue;

	for( int i = 0; i < PointCount - 1; i++ )
	{
		Canvas->DrawLine( Points[ i ], Points[ i + 1 ], 1.0 );
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
	// Add this point to the array
	Points[ PointCount ].X = X;
	Points[ PointCount ].Y = Y;

	XEdit->Text = X;
	YEdit->Text = (PaintBox1->Height - Y) - 20;

	PointCount++;

	PaintBox1->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ClearButtonClick(TObject *Sender)
{
	PointCount = 1;
	Points[ 0 ].X = PaintBox1->Width / 2;
	Points[ 0 ].Y = PaintBox1->Height - 21;

	PaintBox1->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BendButtonClick(TObject *Sender)
{
	float PreviousAngle = 0.0;
	char SendBuf[ 16 ];

	if( fp == NULL )
	{
	   StatusEdit->Text = "Open File First";
	   return;
	}

	// Generate the commands to bend the wire in reverse order of entry
	for( int Segment = PointCount - 1;  Segment > 0; Segment-- )
	{
		// Extrude first, then bend (Can't bend if nothing sticks out)
		// Calculate segment length
		float DeltaX = Points[ Segment ].X - Points[ Segment - 1 ].X;
		float DeltaY = (Points[ Segment ].Y - Points[ Segment - 1 ].Y);
		float Len = sqrt( (DeltaX * DeltaX) + (DeltaY * DeltaY) );
		LengthEdit->Text = Len;

		float Angle = atan( DeltaY/DeltaX );
		Angle = (180 * Angle) / 3.14159; // Radians to degrees
		// Rotate to be relative to extruder exit
		if( Angle < 0 )
			Angle = 90.0 + Angle;
		else
			Angle = Angle - 90.0;

		Angle = Angle - PreviousAngle;
		PreviousAngle = Angle;

		AngleEdit->Text = Angle;

		// Send the bend commands to the Bender
		// Feed first
		sprintf( SendBuf, "F%d\n", (int)Len );
		if( fwrite( SendBuf, 1, strlen( SendBuf ), fp )  != strlen( SendBuf ) )
		{
			StatusEdit->Text = "Error Writing File";
		}

		// Then Bend
		sprintf( SendBuf, "B%d\n", (int)Angle );
		if( fwrite( SendBuf, 1, strlen( SendBuf ), fp )  != strlen( SendBuf ) )
		{
			StatusEdit->Text = "Error Writing File";
		}

	}
}
//---------------------------------------------------------------------------

