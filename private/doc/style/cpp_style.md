# NeKernel C++ Style Guide:

## The Basics:

NeKernel uses the Google C++ Style. But applied to low-level systems. We use C++20 and later throughout all of our stack. As we keep iterating over time we will amend this document.

## No Exceptions, Never.

NeKernel bans the usage of exceptions in vmoskrnl, bootzldr, etc.
Unless it is a program space and not to be deemed performance dependent.

## Templates, Containers, and ZOA over C with Classes.

NeKernel doesn't limit itself to a C++ paradigm, as we may find fitting solutions by exploring other patterns.
That is why we have such containers like TrueResult, FalseResult, etc...


