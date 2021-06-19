# Invoker

*Example for invoking back to the main thread in MFC.*

Some code, including direct UI updates, needs to run directly on the main application 
thread. When application code becomes increasingly parallelized, we need a simple, easy 
pattern to invoke back to the main thread which can be used directly across our applications.

The standard way to invoke in MFC would be to create a new WM_USER message and handler 
for every method which needs to be invoked. This works, but gets very clunky when 
creating a large number of invokes. We can solve this by having the invoker run one 
WM_USER message to use the operating system messaging to get back to the main thread,
and then use a lambda function to make the function calls and variable management, vastly
simplifying the code required to manage the invoke.
