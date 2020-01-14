import React, { Component } from "react";

class Login extends Component {
    
    handleSubmit(event){
        alert('Handle Submit');
        event.preventDefault();
    }

    render(){
        
        return(
            <>
                <div className="header">
                    <h1>DoorLocking</h1>
                    <h2>Please login to open a door</h2>
                    {/* eslint-disable-next-line*/}
                    <h4 id="msg"></h4>
                </div>
                <div class="content">
                    <form onSubmit={this.handleSubmit} className="pure-form pure-form-stacked" name='login' action='/login_handler' method='POST'>
                        <fieldset>
                            <legend>Authentication</legend>
                            <label for='username' >Username:</label>
                            <input type='text' name='username' id='username' maxlength='30' size='30'/>
                            <label for='accessCode'>Acess Code:</label>
                            <input type='password' name='accessCode' id='accessCode' maxlength='30' size='30'/>
                            <button type="submit" className="pure-button pure-button-primary" >Enter</button>
                        </fieldset>
                    </form>
                </div>
            </>

        );
    
    }
}

export default Login;