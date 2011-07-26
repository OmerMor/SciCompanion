//---------------------------------------------------------------------------
/*RESINFO *AddResource(U8 *buf, U16 len, U8 type, S16 number, U8 pack)
{        
	FILE *fMap,*fPack;
    U32 ulMapLen;
    U16 ident;
    U32 addr;
    U32 packOffset;
    U8 *mBuf;

    // load the map
    if((fMap=ssOpenFile(ssFIO_GAME|ssFIO_MESSAGE,"resource.map","rb+"))==0) return NULL;
    ulMapLen = ssFileLen(fMap);
    // make sure the map is valid
    if(ulMapLen < rmMIN_SIZE) {
		ssMessage(ssERROR,"Invalid resource.map file! File too small!");
        ssCloseFile(fMap);
        return FALSE;
    }

    // Ask the user for which package to store the resource and
    // what number to use.
    DlgAddResource = new TDlgAddResource(WndExplorer);
    DlgAddResource->ResourceType = AnsiString(resTypes[type].name);
    DlgAddResource->NumberEdit->Text  = IntToStr(number);
    DlgAddResource->PackageEdit->Text = IntToStr(pack);
    DlgAddResource->NumberEditChange(DlgAddResource->NumberEdit);
    DlgAddResource->ShowModal();
	delete DlgAddResource;

    // they pressed cancel, no res will be added
    if(WndExplorer->dlgCANCLOSE) {
        ssCloseFile(fMap);
        return FALSE;
    }

	number = (S16)WndExplorer->adrsNumber;
	pack = (U8)WndExplorer->adrsPackage;

    // Load the pack
    sprintf(szTemp,"resource.%03d",pack);
    if((fPack=ssOpenFile(ssFIO_GAME|ssFIO_MESSAGE,szTemp,"rb+"))==0) return NULL;
//    ulPackLen = ssFileLen(fPack);

    // move all the current entries forward 6 bytes so that the new entry is at the beginning!
	mBuf = (U8*)ssAlloc(ulMapLen);
    ssFRead(mBuf,ulMapLen,fMap);
    ssFSeek(fMap,6,SEEK_SET);
    ssFWrite(mBuf,ulMapLen,fMap);
    ssFree(mBuf);
    // Now write the new entry!

    // First to the package!
	ident = (U16)rmSET_TYPE(type)|(U16)rmSET_NUMBER(number);
    ssFSeek(fPack,0,SEEK_END);
    packOffset = ftell(fPack);
    ssFPutW(ident,fPack);	 // identifier (same as map)
    ssFPutW(len+4,fPack);  	 // actual length
    ssFPutW(len,fPack);  // encoded length
    ssFPutW(0,fPack);		 // encoding method (0-none)
    ssFWrite(buf,len,fPack);
    ssCloseFile(fPack);
    ssFSeek(fMap,0,SEEK_SET);

	addr = rmSET_PACKAGE(pack)|rmSET_OFFSET(packOffset);
    ssFPutW(ident,fMap);
    ssFPutL(addr,fMap);

    ssCloseFile(fMap);  

	RESINFO *ri = AddResInfo(FindLastResInfo(type), type, pack, number, packOffset);
    ri->size = len;
    ri->encSize = len;
    ri->encType = 0;

    return ri;
}*/