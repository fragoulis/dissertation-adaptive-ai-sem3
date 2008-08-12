#include "CAIManager.h"
#include "kNN.h"
#include "CUnitManager.h"

// ----------------------------------------------------------------------------
CAIManager::CAIManager():
m_iFrequency(80)
{}

// ----------------------------------------------------------------------------
CAIManager::~CAIManager()
{
    kNN::Destroy();
}

// ----------------------------------------------------------------------------
void CAIManager::Read( const char *filename )
{
    IrrXMLReader *xml = createIrrXMLReader(filename);
    assert(xml);

    // read root tag's variables
    xml->read();
    m_iFrequency = xml->getAttributeValueAsInt("frequency");
    
    kNN::Get().Initialize(xml);

    delete xml;
}

// ----------------------------------------------------------------------------
void CAIManager::Update()
{
    CUnit& unit = MgrUnit::Get().GetActor();

    // Create a new case for the current situation
    CCase newcase;
    newcase.Initialize(unit);

    // Compare it with all cases and find the K-most similar cases
    // and choose an action to follow
    kNN::Get().RunAlgo(newcase, unit);
}