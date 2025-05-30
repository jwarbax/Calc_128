//
// Created by war on 5/28/25.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <input.h>
#include <validate.h>
#include <tokenize.h>
#include <calculate.h>
#include <output.h>
#include <quadmath.h>
#include <chrono>
#include <imgui.h>
#include <SDL3/SDL_events.h>

inline string to_string128(__float128 value, int precision = 40) {
    char buf[256];
    char fmt[32];
    snprintf(fmt, sizeof(fmt), "%%.%dQg", precision);
    quadmath_snprintf(buf, sizeof(buf), fmt, value);
    return {buf};
}

string calculationTimeFormat(chrono::microseconds& duration)
{
    using namespace std::chrono;
    ostringstream durationStream;
    if (duration.count()<1000)
    {
        durationStream << "Elapsed Time: " << duration.count() << " µs";
    }
    else if (duration.count()<1000000)
    {
        auto ms = duration_cast<milliseconds>(duration);
        durationStream << "Elapsed Time: " << ms.count() << " ms";
    }
    else
    {
        double seconds = duration.count()/1000000.0;
        durationStream << fixed << setprecision(2);
        durationStream << "Elapsed Time: " << seconds << " s";
    }
    return durationStream.str();
}

void calculationResult(Context& reference)
{
    getRawInput(reference);
    getCleanInput(reference);

    if (SDL_EVENT_KEY_DOWN==SDLK_RETURN && isValidInput(reference))
    {
        auto start{chrono::high_resolution_clock::now()};

        tokenize(reference);
        calculate(reference);

        auto end{chrono::high_resolution_clock::now()};
        auto duration{std::chrono::duration_cast<std::chrono::microseconds>(end - start)};

        ostringstream roundedResult;

        long double doubleResult{static_cast<double>(reference.float128_Result)};

        roundedResult << fixed << setprecision(reference.userPrecision);
        roundedResult << &doubleResult << endl;

        reference.stringResult = roundedResult.str();
        reference.calculationTime=calculationTimeFormat(duration);
    }
    else
    {
        reference.stringResult = "UNDEFINED ERROR(calculationResult)";
        reference.calculationTime.clear();
    }
    ImGui::Text("%s", reference.stringResult);
    reference.rawInput.clear();
    reference.cleanInput.clear();
    reference.rawTokens.clear();
    reference.float128_Result={0};
    reference.stringResult.clear();
    reference.calculationTime.clear();
}
