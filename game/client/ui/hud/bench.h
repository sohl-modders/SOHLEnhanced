#if !defined ( BENCHH )
#define BENCHH
#pragma once

#define FIRST_STAGE		1
#define SECOND_STAGE	2
#define THIRD_STAGE		3
#define FOURTH_STAGE	4
#define LAST_STAGE		( FOURTH_STAGE )

void Bench_CheckStart( void );

int Bench_InStage( int stage );
int Bench_GetPowerPlay( void );
int Bench_GetStage( void );
void Bench_SetPowerPlay( int set );
int Bench_Active( void );

void Bench_SetDotAdded( int dot );
void Bench_SpotPosition( const Vector& dot, const Vector& target );
void Bench_CheckEntity( int type, cl_entity_t *ent, const char *modelname );
void Bench_AddObjects( void );
void Bench_SetViewAngles( int recalc_wander, Vector& viewangles, float frametime, usercmd_t *cmd );
void Bench_SetViewOrigin( Vector& vieworigin, float frametime );

#endif