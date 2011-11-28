#include "../RasterizerState.h"

using namespace GNAFramework;

const RasterizerState RasterizerState::CullClockwise(CullClockwiseFace, Solid);
const RasterizerState RasterizerState::CullCounterClockwise(CullCounterClockwiseFace, Solid);
const RasterizerState RasterizerState::CullNone(None, Solid);


RasterizerState::RasterizerState() {
    this->cullMode = None;//CullClockwiseFace;
    this->fillMode = Solid;
}

RasterizerState::RasterizerState(CullMode cullMode, FillMode fillMode) {
    this->cullMode = cullMode;
    this->fillMode = fillMode;
}



bool RasterizerState::operator ==(const RasterizerState& other) const {
    return (cullMode == other.cullMode) && (fillMode == other.fillMode);
}

bool RasterizerState::operator !=(const RasterizerState& other) const {
    return (cullMode != other.cullMode) || (fillMode != other.fillMode);
}

RasterizerState::~RasterizerState() {
}

