#pragma once

#include "Common/DllExport.h"

#include "afxwin.h"
#undef min
#undef max


class DLL_EXPORT Invoker : public CWnd
{
private:
   Invoker();
public:
   virtual ~Invoker();

   // Configuration
   static Invoker& GetInvoker();
   /// <summary>
   /// Initializes the Invoker. Call before using from the app's main window on the main thread.
   /// </summary>
   /// <param name="parentWnd">A valid window handle to run MFC messages through</param>
   static void Initialize(CWnd* parentWnd);

   DECLARE_MESSAGE_MAP()

public:
   // Utilities
   static bool IsOnMainThread();

   // Invoke
protected:
   afx_msg LRESULT OnInvokeThreadMessage(WPARAM wParam, LPARAM lParam);
public:
   /// <summary>
   /// Invoke to ensure code is running on the main thread.
   /// </summary>
   /// <example>
   /// void Method(int x)
   /// {
   ///   if(Invoker::IsOnMainThread() == false)
   ///   {
   ///      Invoker::Invoke([=]() { Method(x); });
   ///      return;
   ///   }
   /// 
   ///   // Do Method stuff...
   /// }
   /// </example>
   /// <remarks>
   /// You can bind the lambda with [&] if, and only if, there are no member variables.
   /// The prefered method is to use [=] in all circumstances as it gives better thread
   /// safety and prevents your variables from changing due to race conditions which
   /// could be caused by how your program uses memory.
   /// </remarks>
   /// <param name="handler">Lambda to run when invoked to the main thread</param>
   static void Invoke(std::function<void(void)> handler);

private:
   std::thread::id _mainThreadID;
};
