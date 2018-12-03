#include "Lexique.h"
#include <fstream>

Lexique::Lexique():currentState(nullptr)
{

}


Lexique::~Lexique()
{
}

std::vector<QString> Lexique::getItems()
{
    return items;
}

std::vector<State*> Lexique::getStates()
{
    return states;
}

void Lexique::setItem(std::vector<QString> items)
{
    this->items = items;
}

void Lexique::setState(std::vector<State*> states)
{;
this->states = states;
}

void Lexique::addState(State * state)
{
    states.push_back(state);
}

void Lexique::setCurrentState(State * state)
{
    this->currentState = state;
}

State * Lexique::getCurrentState()
{
    if (currentState == nullptr) {
        return states[0];
    }
    return currentState;
}

bool containValue(int value, std::vector<int> list) {
    for (unsigned int i = 0; i < list.size(); i++)
        if (list[i] == value)
            return true;

    return false;
}

std::map<QChar, Branch*> Lexique::createBranchs(int index, std::vector<int> previous_branch_outputs)
{
    std::map<QChar, Branch*> branchs;
    for (unsigned int i = 0; i < items.size(); i++) {
        if (containValue(i, previous_branch_outputs)) {
            if (items[i].length() > index) {
                QChar key = items[i].at(index);

                if (branchs.find(key) == branchs.end()) {
                    branchs[key] = new Branch();
                }

                branchs[key]->addOutput(i);
            }
        }

    }
    return branchs;
}

int Lexique::getBiggerItemCount()
{
    unsigned int max = 0;
    for (int unsigned i = 0; i < items.size(); i++) {
        if (items[i].length() > max) {
            max = items[i].length();
        }
    }
    return max;
}

void Lexique::loadItems(QTextStream& in){
    while(!in.atEnd()){
        items.push_back(in.readLine());
    }
    //ajout du state prison
//    items.push_back("aucun mot correspondant");
};

void Lexique::buildAutomate()
{

    std::vector<State*> currents_states_ids;
    std::vector<State*> next_states_ids;

    //initialise le tableau des etats qu'on va traiter
    //le tableau est vide au debut, donc lui ajoute un etat vide au debut
    currents_states_ids.push_back(new State(nb_state++));

    int maxIndex = getBiggerItemCount();
    int current_position = 0;

    // On va traiter tout mot dans notre lexique, pour chacun de leur QCharactere
    while (current_position < maxIndex) {

        for (State*  current_state  : currents_states_ids) {

            std::vector<int> previous_branchs_output;

            //on verifie qu'on est pas dans le state initial
            if (current_state->getPreviousState() == nullptr) {
                for (unsigned int j = 0; j < items.size(); j++)
                    previous_branchs_output.push_back(j);
            }
            else
                // on doit mettre le state qu'on vient de creer dans la branche qui le mene
                previous_branchs_output = current_state->getPreviousState()->getBranchs()[current_state->getValue()]->getOutput();

            //on cree la nouvelle liste de branche pour l'etat en cours
            std::map<QChar, Branch*> branchs = createBranchs(current_position, previous_branchs_output);


            for (std::map<QChar, Branch*>::iterator it = branchs.begin(); it != branchs.end(); ++it)
            {
                //pour chacune des nouvelles branches on doit creer un nouvel etat
                State* new_state = new State(nb_state++);

                //on assigne l'etat present comme etant l'etat precedent du nouvel etat
                new_state->setPrevious(current_state);
                new_state->setValue(it->first);

                //le nouvel etat est mit alors dans la branche
                it->second->setNextState(new_state);

                //la liste des etat cree est conserve pour qu'on les traite au prochain tour
                next_states_ids.push_back(new_state);
            }

            // une fois la liste de branche traite, on l'ajoute a l'etat present
            current_state->setBranchs(branchs);

            // on ajoute l'etat present a la liste d'etat du lexique
            states.push_back(current_state);
        }


        currents_states_ids = next_states_ids;
        next_states_ids.clear();

        current_position++;
    }

}

