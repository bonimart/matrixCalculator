#pragma once
#include "../settings.h"
#include "Unary/Print.h"
#include "Unary/GEM.h"
#include "Unary/Rank.h"
#include "Unary/Inversion.h"
#include "Unary/Determinant.h"
#include "Unary/Transposition.h"
#include "Binary/Addition.h"
#include "Binary/Subtraction.h"
#include "Binary/Multiplication.h"
#include "Binary/Selection.h"
#include "Binary/Union.h"

const std::unordered_map<std::string, std::shared_ptr<Operation>> operations = {
    std::make_pair<std::string, std::shared_ptr<Operation>>("tr", std::make_shared<Transposition>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("det", std::make_shared<Determinant>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("print", std::make_shared<Print>(std::cout)),
    std::make_pair<std::string, std::shared_ptr<Operation>>("gem", std::make_shared<GEM>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("rank", std::make_shared<Rank>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("inv", std::make_shared<Inversion>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("sel", std::make_shared<Selection>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("join", std::make_shared<Union>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("+", std::make_shared<Addition>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("-", std::make_shared<Subtraction>()),
    std::make_pair<std::string, std::shared_ptr<Operation>>("*", std::make_shared<Multiplication>())};

const std::unordered_map<std::string, int> operators = {
    {SET_VAR, WRITE_PRIORITY},
    {WRITE_TO_FILE, WRITE_PRIORITY},
    {"+", WRITE_PRIORITY + 1},
    {"-", WRITE_PRIORITY + 1},
    {"*", WRITE_PRIORITY + 2}};