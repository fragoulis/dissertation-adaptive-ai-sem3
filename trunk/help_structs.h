#pragma once

class CCase;

// ----------------------------------------------------------------------------
struct _case
{
    _case():
        distance(INT_MAX),
        oCase(0)
    {}

    int distance;
    CCase *oCase;
};

// ----------------------------------------------------------------------------
struct _action
{
    _action():
        votes(0),
        avg_fitness(0)
    {}

    E_ACTION action;
    int votes;
    double avg_fitness;
};