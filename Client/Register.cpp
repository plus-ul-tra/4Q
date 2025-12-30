#include "pch.h"
extern void LinkEngineComponents();

static bool ForceLink = []() {
    LinkEngineComponents();
    return true;
    }();