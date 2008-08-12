#pragma once
#include "Singleton.h"
#include "CCase.h"
#include "CCasePath.h"
#include "help_structs.h"
#include <vector>
#include <string>

class kNN : public Singleton<kNN>, public IRenderable
{
private:
    friend Singleton<kNN>;
    typedef std::vector<CCase> CaseBase;
    typedef std::vector<CCasePath> CasePaths;
    typedef std::vector<_case> _k_cases;

    //! The game's case base.
    CaseBase m_casebase;

    //! The game's history of paths followed by the agent.
    //! Used to extract statistics of completeness regarding the algorithm
    //! and for tweeking the fitness for the cases.
    CasePaths m_casepaths;

    //! Algorithm's states.
    enum ALGO_STATE
    {
        ALGO_STARTING,
        ALGO_RUNNING,
        ALGO_STOPPING
    };

    //! Algorithm's current state.
    ALGO_STATE m_state;

    //! The K number.
    int K;

    //! The threshold below which we update a case history.
    double m_updateThreshold;

    //! The file to output the casebase to and load it from.
    std::string m_fileCasebase;

    //! The file to output the results.
    std::string m_fileResults;

    //! The current run's fitness.
    double m_fitness;

    //! The list of K selected cases.
    _k_cases m_k_cases;

public:
    //! Compares a new case with all cases in the case base.
    //! Returns the action the new case should follow.
    void RunAlgo( CCase &newcase, CUnit &unit );

    //! Adds a new case.
    void AddCase( const CCase &newcase ) { m_casebase.push_back(newcase); }

    //! Writes out the case data in a string.
    void Write( stringw &out ) const;

    //! Saves all cases to file.
    void Save() const;

    //! Saves test results to file.
    void SaveResults() const;

    //! Loads a case base from file.
    void Load();

    //! Empties case base.
    void Empty() { m_casebase.clear(); }

    //! Initializes the algorithm.
    void Initialize( IrrXMLReader *xml );

    //! Debugs the algorithm graphically.
    virtual void Render() const;

private:
    //! Constructor.
    kNN();

    //! Destructor.
    ~kNN();

    //! Sorts actions by fitness.
    void _SortActions( std::vector<_action> &actions ) const;

    //! Checks if algorithm is starting.
    bool _IsAlgoStarting() const { return ( ALGO_STARTING == m_state ); }

    //! Checks if algorithm is running.
    bool _IsAlgoRunning() const { return ( ALGO_RUNNING == m_state ); }

    //! Checks if algorithm is stopping.
    bool _IsAlgoStopping() const { return ( ALGO_STOPPING == m_state ); }

    //! Runs the algorithm for learning randomly.
    void _RunRandomly( CCase &newcase, CUnit &unit );

    //! Runs the algorithm for learning and adapting.
    void _RunNormal( CCase &newcase, CUnit &unit );

    //! Final steps of the algorithm.
    void _Finalize( CCasePath &cp, CCase *c );

    //! First steps of the algorithm.
    CCasePath& _Initialize( CUnit &unit );

    //! Checks the resulting fitness.
    void _CheckFitness();
};