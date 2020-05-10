//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef IVOICETWEAK_H
#define IVOICETWEAK_H
#ifdef _WIN32
#pragma once
#endif

/**
*	These provide access to the voice controls.
*/
enum VoiceTweakControl
{
	/**
	*	Values 0-1.
	*/
	MicrophoneVolume	= 0,	

	/**
	*	Values 0-1. Scales how loud other players are.
	*/
	OtherSpeakerScale,

	/**
	*	20 db gain to voice input.
	*/
	MicBoost,
};

/**
*	Voice tweak API.
*	These turn voice tweak mode on and off. While in voice tweak mode, the user's voice is echoed back
*	without sending to the server. 
*/
struct IVoiceTweak
{
	/**
	*	Starts transmitting voice.
	*	@return 0 on error.
	*/
	int				(*StartVoiceTweakMode)();

	/**
	*	Stops transmitting voice.
	*/
	void			(*EndVoiceTweakMode)();
	
	/**
	*	Sets the value for the given control.
	*	@param control Control to set.
	*	@param value Value to set.
	*/
	void			(*SetControlFloat)( VoiceTweakControl iControl, float value );

	/**
	*	Gets the value for the given control.
	*	@param control Control to get.
	*	@return Value.
	*/
	float			(*GetControlFloat)( VoiceTweakControl iControl );

	/**
	*	@return The speaking volume.
	*/
    int             (*GetSpeakingVolume)();
};


#endif // IVOICETWEAK_H
