//TODO: info + some doc

// abstract base class - cannot be created
class AbstractAITargetCallbacks
{
};

class AITargetCallbacks : AbstractAITargetCallbacks
{
	vector GetHeadPositionWS() { return vector.Zero; }
	vector GetVisionPointPositionWS(EntityAI pApplicant) { return vector.Zero; }
	
	float GetMaxVisionRangeModifier(EntityAI pApplicant) { return 1.0; }
};