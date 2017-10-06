/**********************************************************************

  Audacity: A Digital Audio Editor

  TimeDialog.cpp

  Dominic Mazzoni

*******************************************************************//**

\class TimeDialog
\brief Dialog used to request a time value.

*//*******************************************************************/

#include "Audacity.h"

#include <wx/defs.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/string.h>

#include "widgets/NumericTextCtrl.h"
#include "ShuttleGui.h"
#include "TimeDialog.h"

BEGIN_EVENT_TABLE(TimeDialog, wxDialogWrapper)
   EVT_COMMAND(wxID_ANY, EVT_TIMETEXTCTRL_UPDATED, TimeDialog::OnUpdate)
END_EVENT_TABLE()

TimeDialog::TimeDialog(wxWindow *parent,
                       const wxString &title,
                       const NumericFormatId &format,
                       double rate,
                       double time,
                       const wxString &prompt)
:  wxDialogWrapper(parent, wxID_ANY, title),
   mPrompt(prompt),
   mFormat(format),
   mRate(rate),
   mTime(time),
   mTimeCtrl(NULL)
{
   SetName(GetTitle());
   ShuttleGui S(this, eIsCreating);
   PopulateOrExchange(S);
}

void TimeDialog::PopulateOrExchange(ShuttleGui &S)
{
   S.SetBorder(5);
   S.StartVerticalLay(true);
   {
      S.StartStatic(mPrompt, true);
      {
         mTimeCtrl = safenew
            NumericTextCtrl(
               this, wxID_ANY,
                         NumericConverter::TIME,
                         mFormat,
                         mTime,
                         mRate,
                         NumericTextCtrl::Options{}
                            .AutoPos(true));
         mTimeCtrl->SetName(mPrompt);
         S.AddWindow(mTimeCtrl);
      }
      S.EndStatic();
   }
   S.EndVerticalLay();
   S.AddStandardButtons();

   TransferDataToWindow();

   Layout();
   Fit();
   SetMinSize(GetSize());
   Center();
}

bool TimeDialog::TransferDataToWindow()
{
   mTimeCtrl->SetFormatString(mTimeCtrl->GetBuiltinFormat(mFormat));
   mTimeCtrl->SetSampleRate(mRate);
   mTimeCtrl->SetValue(mTime);
   mTimeCtrl->SetFocus();

   return true;
}

bool TimeDialog::TransferDataFromWindow()
{
   mTime = mTimeCtrl->GetValue();

   return true;
}

const double TimeDialog::GetTimeValue()
{
   return mTime;
}

void TimeDialog::SetFormatString(const NumericFormatId &formatString)
{
   mFormat = formatString;
   TransferDataToWindow();
}

void TimeDialog::SetSampleRate(double sampleRate)
{
   mRate = sampleRate;
   TransferDataToWindow();
}

void TimeDialog::SetTimeValue(double newTime)
{
   mTime = newTime;
   TransferDataToWindow();
}

void TimeDialog::OnUpdate(wxCommandEvent &event)
{
   Layout();
   Refresh();

   event.Skip(false);
}
