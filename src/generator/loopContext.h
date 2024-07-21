// Copyright 2024 Lucas Norman

#pragma once

#include <vector>

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include "../parser/ast.h"

#include "../diagnostics/generator.h"

namespace loopContext {
// to keep track of all loop information when generating break and continue
// statements
struct LoopContext {
    ASTNode* updateStatement;
    llvm::BasicBlock* conditionalBlock;
    llvm::BasicBlock* mergeBlock;

    [[nodiscard]] inline bool isForLoop() const {
        return updateStatement != nullptr;
    }
};

// to keep track of information for the current loops, so that break and
// continue statements can be generated correctly
static std::vector<LoopContext> loopContexts;

inline void startForLoop(ASTNode* updateStatement,
                         llvm::BasicBlock* conditionalBlock,
                         llvm::BasicBlock* mergeBlock) {
    // save loop information for potential continue and break
    loopContexts.emplace_back(updateStatement, conditionalBlock, mergeBlock);
}

inline void startWhileLoop(llvm::BasicBlock* conditionalBlock,
                           llvm::BasicBlock* mergeBlock) {
    // save loop information for potential continue and break
    loopContexts.emplace_back(nullptr, conditionalBlock, mergeBlock);
}

inline void endLoop() {
    // restore the loop information for potential continue and break statements
    loopContexts.pop_back();
}

inline bool currentlyNotInsideLoop() { return loopContexts.empty(); }

inline LoopContext& getCurrentLoopContext() { return loopContexts.back(); }
}  // namespace loopContext
