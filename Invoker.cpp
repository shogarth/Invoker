#include "stdafx.h"
#include "Invoker.h"

#include "Common/WMUserMessages.h"


Invoker::Invoker()
{
   _mainThreadID = std::this_thread::get_id();
}

Invoker::~Invoker() {}

Invoker& Invoker::GetInvoker()
{
   static Invoker invoker;
   return invoker;
}


void Invoker::Initialize(CWnd* parentWnd)
{
   GetInvoker().Create(nullptr, nullptr, NULL, CRect(0, 0, 0, 0), parentWnd, NULL, NULL);
}


BEGIN_MESSAGE_MAP(Invoker, CWnd)
   ON_MESSAGE(WM_INVOKE_THREAD_MSG, OnInvokeThreadMessage)
END_MESSAGE_MAP()


bool Invoker::IsOnMainThread()
{
   auto& invoker = GetInvoker();
   return std::this_thread::get_id() == invoker._mainThreadID;
}


struct InvokeDataWrapper
{
   std::function<void(void)> handler;
};


LRESULT Invoker::OnInvokeThreadMessage(WPARAM wParam, LPARAM lParam)
{
   auto wrapper = reinterpret_cast<InvokeDataWrapper*>(lParam);
   throw_assert(wrapper != nullptr, "No invoke data available");
   if (wrapper != nullptr)
   {
      wrapper->handler();
	  delete wrapper;
      return TRUE;
   }
   return FALSE;
}


void Invoker::Invoke(std::function<void(void)> handler)
{
   auto wrapper = new InvokeDataWrapper();
   wrapper->handler = handler;

   GetInvoker().PostMessage(WM_INVOKE_THREAD_MSG, (WPARAM)nullptr, reinterpret_cast<LPARAM>(wrapper));
}
