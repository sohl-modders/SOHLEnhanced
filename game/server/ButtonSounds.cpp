#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ButtonSounds.h"

// Button sound table. 
// Also used by CBaseDoor to get 'touched' door lock/unlock sounds

const char* ButtonSound( int sound )
{
	const char* pszSound;

	switch( sound )
	{
	case 0: pszSound = "common/null.wav";		break;
	case 1: pszSound = "buttons/button1.wav";	break;
	case 2: pszSound = "buttons/button2.wav";	break;
	case 3: pszSound = "buttons/button3.wav";	break;
	case 4: pszSound = "buttons/button4.wav";	break;
	case 5: pszSound = "buttons/button5.wav";	break;
	case 6: pszSound = "buttons/button6.wav";	break;
	case 7: pszSound = "buttons/button7.wav";	break;
	case 8: pszSound = "buttons/button8.wav";	break;
	case 9: pszSound = "buttons/button9.wav";	break;
	case 10: pszSound = "buttons/button10.wav";	break;
	case 11: pszSound = "buttons/button11.wav";	break;
	case 12: pszSound = "buttons/latchlocked1.wav";	break;
	case 13: pszSound = "buttons/latchunlocked1.wav";	break;
	case 14: pszSound = "buttons/lightswitch2.wav"; break;

		// next 6 slots reserved for any additional sliding button sounds we may add

	case 21: pszSound = "buttons/lever1.wav";	break;
	case 22: pszSound = "buttons/lever2.wav";	break;
	case 23: pszSound = "buttons/lever3.wav";	break;
	case 24: pszSound = "buttons/lever4.wav";	break;
	case 25: pszSound = "buttons/lever5.wav";	break;

	default:pszSound = "buttons/button9.wav";	break;
	}

	return pszSound;
}

const char* DoorMoveSound( const int iSound )
{
	switch( iSound )
	{
	case 1:		return "doors/doormove1.wav";
	case 2:		return "doors/doormove2.wav";
	case 3:		return "doors/doormove3.wav";
	case 4:		return "doors/doormove4.wav";
	case 5:		return "doors/doormove5.wav";
	case 6:		return "doors/doormove6.wav";
	case 7:		return "doors/doormove7.wav";
	case 8:		return "doors/doormove8.wav";
	case 9:		return "doors/doormove9.wav";
	case 10:	return "doors/doormove10.wav";
	case 0:
	default:	return "common/null.wav";
	}
}

const char* DoorStopSound( const int iSound )
{
	switch( iSound )
	{
	case 1:		return "doors/doorstop1.wav";
	case 2:		return "doors/doorstop2.wav";
	case 3:		return "doors/doorstop3.wav";
	case 4:		return "doors/doorstop4.wav";
	case 5:		return "doors/doorstop5.wav";
	case 6:		return "doors/doorstop6.wav";
	case 7:		return "doors/doorstop7.wav";
	case 8:		return "doors/doorstop8.wav";
	case 0:
	default:	return "common/null.wav";
	}
}

const char* LockedSentence( const int iSound )
{
	switch( iSound )
	{
	case 1: return "NA"; // access denied
	case 2: return "ND"; // security lockout
	case 3: return "NF"; // blast door
	case 4: return "NFIRE"; // fire door
	case 5: return "NCHEM"; // chemical door
	case 6: return "NRAD"; // radiation door
	case 7: return "NCON"; // gen containment
	case 8: return "NH"; // maintenance door
	case 9: return "NG"; // broken door

	default: return nullptr;
	}
}

const char* UnlockedSentence( const int iSound )
{
	switch( iSound )
	{
	case 1: return "EA"; // access granted
	case 2: return "ED"; // security door
	case 3: return "EF"; // blast door
	case 4: return "EFIRE"; // fire door
	case 5: return "ECHEM"; // chemical door
	case 6: return "ERAD"; // radiation door
	case 7: return "ECON"; // gen containment
	case 8: return "EH"; // maintenance door

	default: return nullptr;
	}
}

#define DOOR_SENTENCEWAIT	6
#define DOOR_SOUNDWAIT		3
#define BUTTON_SOUNDWAIT	0.5

// play door or button locked or unlocked sounds. 
// pass in pointer to valid locksound struct. 
// if flocked is true, play 'door is locked' sound,
// otherwise play 'door is unlocked' sound
// NOTE: this routine is shared by doors and buttons

void PlayLockSounds( CBaseEntity* pEntity, locksound_t *pls, const bool bLocked, const bool bButton )
{
	// LOCKED SOUND

	// CONSIDER: consolidate the locksound_t struct (all entries are duplicates for lock/unlock)
	// CONSIDER: and condense this code.
	float flsoundwait;

	if( bButton )
		flsoundwait = BUTTON_SOUNDWAIT;
	else
		flsoundwait = DOOR_SOUNDWAIT;

	if( bLocked )
	{
		const bool fplaysound = ( pls->sLockedSound && gpGlobals->time > pls->flwaitSound );
		const bool fplaysentence = ( pls->sLockedSentence && !pls->bEOFLocked && gpGlobals->time > pls->flwaitSentence );
		float fvol;

		if( fplaysound && fplaysentence )
			fvol = 0.25;
		else
			fvol = 1.0;

		// if there is a locked sound, and we've debounced, play sound
		if( fplaysound )
		{
			// play 'door locked' sound
			EMIT_SOUND( pEntity, CHAN_ITEM, ( char* ) STRING( pls->sLockedSound ), fvol, ATTN_NORM );
			pls->flwaitSound = gpGlobals->time + flsoundwait;
		}

		// if there is a sentence, we've not played all in list, and we've debounced, play sound
		if( fplaysentence )
		{
			// play next 'door locked' sentence in group
			int iprev = pls->iLockedSentence;

			pls->iLockedSentence = g_Sentences.PlaySequentialSz( pEntity, STRING( pls->sLockedSentence ),
															   0.85, ATTN_NORM, 0, 100, pls->iLockedSentence, false );
			pls->iUnlockedSentence = 0;

			// make sure we don't keep calling last sentence in list
			pls->bEOFLocked = ( iprev == pls->iLockedSentence );

			pls->flwaitSentence = gpGlobals->time + DOOR_SENTENCEWAIT;
		}
	}
	else
	{
		// UNLOCKED SOUND

		const bool bPlaysound = ( pls->sUnlockedSound && gpGlobals->time > pls->flwaitSound );
		const bool bPlaysentence = ( pls->sUnlockedSentence && !pls->bEOFUnlocked && gpGlobals->time > pls->flwaitSentence );
		float fvol;

		// if playing both sentence and sound, lower sound volume so we hear sentence
		if( bPlaysound && bPlaysentence )
			fvol = 0.25;
		else
			fvol = 1.0;

		// play 'door unlocked' sound if set
		if( bPlaysound )
		{
			EMIT_SOUND( pEntity, CHAN_ITEM, ( char* ) STRING( pls->sUnlockedSound ), fvol, ATTN_NORM );
			pls->flwaitSound = gpGlobals->time + flsoundwait;
		}

		// play next 'door unlocked' sentence in group
		if( bPlaysentence )
		{
			int iprev = pls->iUnlockedSentence;

			pls->iUnlockedSentence = g_Sentences.PlaySequentialSz( pEntity, STRING( pls->sUnlockedSentence ),
																 0.85, ATTN_NORM, 0, 100, pls->iUnlockedSentence, false );
			pls->iLockedSentence = 0;

			// make sure we don't keep calling last sentence in list
			pls->bEOFUnlocked = ( iprev == pls->iUnlockedSentence );
			pls->flwaitSentence = gpGlobals->time + DOOR_SENTENCEWAIT;
		}
	}
}