#ifndef WAVE_DATABASE_H
#define WAVE_DATABASE_H

typedef struct WaveDbResult WaveDbResult;

void dbConnect(const char *connectionString);
WaveDbResult* dbQuery(const char *query);
void dbFreeResult(WaveDbResult *result);
void dbClose();

#endif // WAVE_DATABASE_H