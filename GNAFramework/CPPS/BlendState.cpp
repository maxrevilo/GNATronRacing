#include "../BlendState.h"

using namespace GNAFramework;

const BlendState BlendState::Additive(SourceAlpha, One, Add);

const BlendState BlendState::AlphaBlend(One, InverseSourceAlpha, Add);

const BlendState BlendState::NonPremultiplied(SourceAlpha, InverseSourceAlpha, Add);

const BlendState BlendState::Opaque(One, Zero, Add);

BlendState::BlendState() {

    this->SourceBlend = SourceAlpha;
    this->DestinationBlend = InverseSourceAlpha;
    this->ColorBlendFunction = Add;

}

BlendState::BlendState(Blend SourceBlend, Blend DestinationBlend,
        BlendFunction ColorBlendFunction) {

    this->SourceBlend = SourceBlend;
    this->DestinationBlend = DestinationBlend;
    this->ColorBlendFunction = ColorBlendFunction;

}

bool BlendState::operator ==(const BlendState& other) const {
    return SourceBlend == other.SourceBlend &&
            DestinationBlend == other.DestinationBlend &&
            ColorBlendFunction == other.ColorBlendFunction;
}

bool BlendState::operator !=(const BlendState& other) const {
    return SourceBlend != other.SourceBlend ||
            DestinationBlend != other.DestinationBlend ||
            ColorBlendFunction != other.ColorBlendFunction;
}

//BlendState::BlendState(const BlendState& orig) {}

//BlendState::~BlendState() {}



