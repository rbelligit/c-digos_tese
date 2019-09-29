
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include "./decode/CalcSimbToBitsMsg.h"

int main(int argc, char *argv[])
{
    CalcSimbToBitsMsg calc;

    /**
     * probsSimbs -> Probabilidade de cada símbolo, dado o nível recebido.
     * msgsFromBits -> mensagens dos bits para este símbolo
     * bitN -> para qual bit esta sendo calculada a mensagem, de 0 até nBits-1 (nBits = msgsFromBits.size())
     */
    //double CalcSimbToBitsMsg::calcFuncsimbToBit(std::vector<double>& probsSimbs,
    //		std::vector<double>& msgsFromBits, int bitN)

    std::vector<double> probs;
    std::vector<double> msgs;
    probs.resize(2);
    msgs.resize(2);
    probs[0] = 0.9;
    probs[1] = 0.1;
    msgs[0] = 1;
    msgs[1] = 1;
    std::vector<uint32_t> vals{0, 0, 0, 1};
    calc.setMapping(&vals);
    double val1 = calc.calcFuncsimbToBit(probs, msgs, 0);
    double val2 = calc.calcFuncsimbToBit(probs, msgs, 1);
    std::cout << "Resultado . val1=" << val1 << " - val2 =" << val2 << std::endl;
    return 0;
}