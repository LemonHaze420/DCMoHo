void LookupError(LONG _Code, TCHAR* _Buffer)
{
	switch(_Code)
	{
	case 0:
		strcat(_Buffer, "Command completed successfully. (DI_OK)");
		break;
	case S_FALSE:
		strcat(_Buffer, "Command failed. (S_FALSE). Could be caused by any one of the following:\n");
		strcat(_Buffer, "DI_NOTATTACHED\n");
		strcat(_Buffer, "DI_BUFFEROVERFLOW\n");
		strcat(_Buffer, "DI_PROPNOEFFECT\n");
		strcat(_Buffer, "DI_NOEFFECT\n");
		strcat(_Buffer, "DISCL_EXCLUSIVE\n");
		break;
	case DI_POLLEDDEVICE:
		strcat(_Buffer, "DI_POLLEDDEVICE");
		break;
	case DI_DOWNLOADSKIPPED:
		strcat(_Buffer, "DI_DOWNLOADSKIPPED");
		break;
	case DI_EFFECTRESTARTED:
		strcat(_Buffer, "DI_EFFECTRESTARTED");
		break;
//	case DI_SETTINGSNOTSAVED_ACCESSDENIED:
//		strcat(_Buffer, "DI_SETTINGSNOTSAVED_ACCESSDENIED");
//		break;
//	case DI_SETTINGSNOTSAVED_DISKFULL:
//		strcat(_Buffer, "DI_SETTINGSNOTSAVED_DISKFULL");
//		break;
	case DI_TRUNCATED:
		strcat(_Buffer, "DI_TRUNCATED");
		break;
	case DI_TRUNCATEDANDRESTARTED:
		strcat(_Buffer, "DI_TRUNCATEDANDRESTARTED");
		break;
//	case DI_WRITEPROTECT:
//		strcat(_Buffer, "DI_WRITEPROTECT");
//		break;
	case DIERR_OLDDIRECTINPUTVERSION:
		strcat(_Buffer, "DIERR_OLDDIRECTINPUTVERSION");
		break;
	case DIERR_BETADIRECTINPUTVERSION:
		strcat(_Buffer, "DIERR_BETADIRECTINPUTVERSION");
		break;
	case DIERR_BADDRIVERVER:
		strcat(_Buffer, "DIERR_BADDRIVERVER");
		break;
	case DIERR_DEVICENOTREG:
		strcat(_Buffer, "DIERR_DEVICENOTREG");
		break;
	case DIERR_NOTFOUND:
		strcat(_Buffer, "DIERR_NOTFOUND or DIERR_OBJECTNOTFOUND");
		break;
	case DIERR_INVALIDPARAM:
		strcat(_Buffer, "DIERR_INVALIDPARAM");
		break;
	case DIERR_NOINTERFACE:
		strcat(_Buffer, "DIERR_NOINTERFACE");
		break;
	case DIERR_GENERIC:
		strcat(_Buffer, "DIERR_GENERIC");
		break;
	case DIERR_OUTOFMEMORY:
		strcat(_Buffer, "DIERR_OUTOFMEMORY");
		break;
	case DIERR_UNSUPPORTED:
		strcat(_Buffer, "DIERR_UNSUPPORTED");
		break;
	case DIERR_NOTINITIALIZED:
		strcat(_Buffer, "DIERR_NOTINITIALIZED");
		break;
	case DIERR_ALREADYINITIALIZED:
		strcat(_Buffer, "DIERR_ALREADYINITIALIZED");
		break;
	case DIERR_NOAGGREGATION:
		strcat(_Buffer, "DIERR_NOAGGREGATION");
		break;
	case DIERR_OTHERAPPHASPRIO:
		strcat(_Buffer, "DIERR_OTHERAPPHASPRIO, DIERR_READONLY, or DIERR_HANDLEEXISTS");
		break;
	case DIERR_INPUTLOST:
		strcat(_Buffer, "DIERR_INPUTLOST");
		break;
	case DIERR_ACQUIRED:
		strcat(_Buffer, "DIERR_ACQUIRED");
		break;
	case DIERR_NOTACQUIRED:
		strcat(_Buffer, "DIERR_NOTACQUIRED");
		break;
	case DIERR_INSUFFICIENTPRIVS:
		strcat(_Buffer, "DIERR_INSUFFICIENTPRIVS");
		break;
	case DIERR_DEVICEFULL:
		strcat(_Buffer, "DIERR_DEVICEFULL");
		break;
	case DIERR_MOREDATA:
		strcat(_Buffer, "DIERR_MOREDATA");
		break;
	case DIERR_NOTDOWNLOADED:
		strcat(_Buffer, "DIERR_NOTDOWNLOADED");
		break;
	case DIERR_HASEFFECTS:
		strcat(_Buffer, "DIERR_HASEFFECTS");
		break;
	case DIERR_NOTEXCLUSIVEACQUIRED:
		strcat(_Buffer, "DIERR_NOTEXCLUSIVEACQUIRED");
		break;
	case DIERR_INCOMPLETEEFFECT:
		strcat(_Buffer, "DIERR_INCOMPLETEEFFECT");
		break;
	case DIERR_NOTBUFFERED:
		strcat(_Buffer, "DIERR_NOTBUFFERED");
		break;
	case DIERR_EFFECTPLAYING:
		strcat(_Buffer, "DIERR_EFFECTPLAYING");
		break;
	case DIERR_UNPLUGGED:
		strcat(_Buffer, "DIERR_UNPLUGGED");
		break;
	case DIERR_REPORTFULL:
		strcat(_Buffer, "DIERR_REPORTFULL");
		break;
	default:
		sprintf(_Buffer, "%s The error code, %d, could not be translated. This file may be out of date!", _Code);
		break;
	}
}


// Print out the meaning of a DirectX error code.
void PrintDXError(LONG _Code)
{
	char buffer[1024];
	*buffer = 0;
	LookupError(_Code, buffer);
	OutputDebugString(buffer);
	OutputDebugString("\n");
}