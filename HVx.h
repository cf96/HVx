/*
These functions can be used to perform memory edits to the Xbox 360 Hypervisor.
The HvxGetVersion system call was overwritten by the XeBuild devs to allow HV memory access,
and these wrappers will perform the system call with ease.
Written by Cory
*/
DWORD magic = 0x72627472;
__int64 addr;

__int64 __declspec(naked) HvxGetVersion(DWORD key, __int64 type, __int64 SourceAddress, __int64 DestAddress, __int64 lenInBytes)
{
        __asm
        {
                li r0, 0x0
                sc
                blr
        }
}

DWORD HvxPeekDword(ULONGLONG Address)
{
	DWORD *data = 0;
	__int64 dest = 0x8000000000000000ULL + (DWORD)MmGetPhysicalAddress((DWORD*)data);
	HvxGetVersion(magic, 5, Address, dest, 4); 
	return *(DWORD*)data;
}

BYTE* HvxPeekBytes(ULONGLONG Address, DWORD length){
	BYTE * data = (BYTE*)XPhysicalAlloc( length, MAXULONG_PTR, NULL, PAGE_READWRITE ); //make sure to free this memory when you are done with it!
    	addr = 0x8000000000000000ULL + (DWORD)MmGetPhysicalAddress((DWORD*)data);
    	HvxGetVersion( 0x72627472, 5, Address, addr, length );
	return data;
}

void HvxPokeBytes(ULONGLONG Address, BYTE *Data, DWORD length)
{
	__int64 src = 0x8000000000000000ULL + (DWORD)MmGetPhysicalAddress(Data);
	HvxGetVersion(magic, 5, src, Address, length);
}
