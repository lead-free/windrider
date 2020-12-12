//! CommandParser declaration file.
/**
 * @file      CommandParser.h
 * @author    Stanislav Sotnikov (stanislav.sotnikov145@gmail.com)
 *
 */

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>
#include <algorithm>
#include <functional>

#include "HardwareDriver.h"

namespace CommandParser{

    //! Command container class.
    /**
     *  @brief This container is used to place commands into the command lookup table.
     */
    class Cmd {

        public:

        //! Default Cmd constructor.
        /**
         * @param token Command token used to call the specified command.
         * @param help  Help string.
         * @param cmd_handler Function to be executed when command with the token is called.
         */ 
        Cmd(std::string token, std::string help, const std::function<void(const std::vector<std::string>&)> cmd_handler):
            token(token),help(help),cmd_handler(cmd_handler){};

        //! execute method.
        /**
         * @brief Public method to call the command handler.
         * @param arg Argument string passed to the command handler. 
         */ 
        void execute(const std::vector<std::string> &args) const{

            cmd_handler(args);
        };
        
        const std::string token; // Command token.
        const std::string help; // Help string.

        private:

        // Command Handler function pointer.
        const std::function<void(const std::vector<std::string>&)> cmd_handler;
    };

    //! Execute method.
    /**
     * @brief This method processes a raw received command.
     *          1. Seperate token and space seperated args from the raw command.
     *          2. Lookup the token in the command table.
     *          3. In case the token exists in the table, 
     *             execute the corresponding commnad handler passin the space seperated argument.
     *          4. Return a status string.
     * @param arg Raw command to execute as received from host.
     * @return    Status string.
     */ 
    const std::string& execute(const std::string &command);


    //! Parse Tokens method
    /**
     * @brief Tokenizes a string. Pushes the tokens onto args_vector.
     * @tparam delimiter Character seperating tokens.
     * @tparam max_nuof_tokens Max number of tokens allowed.
     * @param arg_string String to be tokenized.
     * @param args_vector Output vector of tokens.
     */
    template<char delimiter, int max_nuof_tokens>
    bool parse_tokens(const std::string& args_string, std::vector<std::string> &args_vector){

        std::string::size_type last_pos = 0;

        while(last_pos < args_string.size()){

            auto separator = args_string.find_first_of(delimiter, last_pos);

            if(separator == std::string::npos)
                separator = args_string.size()-1;

            args_vector.push_back(args_string.substr(last_pos, separator-last_pos));
            
            if(args_vector.size() > max_nuof_tokens)
                return false;
            
            last_pos = separator + 1;
        }

        return true;
    }

    // Command table. Keep alphabetical order.
    // TODO: Figure out const correctness
    const std::array<Cmd, 7> command_table = {
        Cmd("current", " \t-- print suction motor current:\n\r\t\t\t\t1) current", HardwareDriver::suction_power),
        Cmd("led", " \t\t-- set led current:\n\r\t\t\t\t1) led [0-3](#channel) [0-1500](current mA) -- set led current in mA.\
                                            \n\r\t\t\t\t2) led [0-3](#channel) [on/off] -- led channel on/off.\
                                            \n\r\t\t\t\t3) led [on/off] -- all led channels on/off.", HardwareDriver::led),
        Cmd("servo", " \t\t-- set servo angle:\n\r\t\t\t\t1) servo [0-1](#channel) [0,180](rotation deg).", HardwareDriver::suction_power),
        Cmd("solenoid", " \t-- configure impactor solenoids:\n\r\t\t\t\t1) solenoid [0-1](#channel) [on/off] -- impactor on/off.\
                                                            \n\r\t\t\t\t2) solenoid [0-1](#channel) [0-10000](off time ms) [0-10000](on time ms) -- set simpactor timing.", HardwareDriver::solenoid),
        Cmd("suction", "\t\t-- configure suction:\n\r\t\t\t\t1) suction [on/off] -- enable/disable suction motor (this command physically cuts off power).\
                                                \n\r\t\t\t\t2) suction [0-100] -- set suction power in %", HardwareDriver::suction_power),
        Cmd("uart", "\t\t-- setup RS-232 forwarding:\n\r\t\t\t\t1) uart [forward] [\"Command\"] -- forward a string via uart\
                                                     \n\r\t\t\t\t2) uart [reply] [on/off] -- print incoming feedback", HardwareDriver::suction_power),
        Cmd("x", "\t\t-- motor driver motion:\n\r\t\t\t\t1) x [0-230] Travel to absolute position mm\
                                             \n\r\t\t\t\t1) x [home] Find home.", HardwareDriver::stepper)
    };

    // Status strings.
    const std::string unknown_command = "unknown command\n\r";
    const std::string invalid_args = "invalid arguments\n\r";
    const std::string invalid_command = "invalid command enter '?' for help\n\r";
    const std::string welcome_str = "CCNY Robotics Lab Welcomes You, human..\n\r";

    // Generate a global helpstring.
    const std::string help_str = [](){

        std::string help_str = welcome_str;

        // Return a string with all the tokens and help strings.
        std::for_each(command_table.begin(),
                        command_table.end(),
                        [&help_str](const Cmd &cmd_table_entry){

                            help_str += cmd_table_entry.token;
                            help_str += cmd_table_entry.help;
                            help_str += "\n\r";
                        });

        return help_str;
    }();
};

#endif //COMMANDPARSER_H
