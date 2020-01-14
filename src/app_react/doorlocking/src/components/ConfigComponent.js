import React, { Component } from "react";

class Config extends Component {
    
    handleSubmit(event){
        alert('Handle Submit');
        event.preventDefault();
    }


    render(){
        
        return(
            <>
                <div className="header">
                    <h1>Network Configuration</h1>
                    <h3>Fill the form bellow to make your device available on your WiFi network.</h3>
                    {/* eslint-disable-next-line*/}
                    <h4 id="msg"></h4>
                </div>
                <div className="content">
                    <form className="pure-form pure-form-stacked" name="config_form" method="POST" action="/config_handler">
                        <label for="hostname">Door Name:</label>
                        <input type="text" id="hostname" name="hostname"/>
                        <label for="ssid">WiFi Network Name (SSID):</label>
                        <input type="text" id="ssid" name="ssid"/>
                        <label for="pwd">WiFi Password:</label>
                        <input type="text" id="pwd" name="pwd"/>
                        <button type="submit" className="pure-button pure-button-primary" value="Send">Send</button>
                    </form>
                </div>
            </>
        );
    }

}

export default Config;
