import React, { Component } from "react";

class RegisterUser extends Component{

    handleSubmit(event){
        alert('Handle Submit');
        event.preventDefault();
    }

    render(){
        return(
            <>
                <div className="header">
                    <h1>New User</h1>
                    {/* eslint-disable-next-line*/}
                    <h4 id="msg"></h4>
                </div>
                <div className="content">
                    <form className="pure-form " name='login' onSubmit={this.handleSubmit} action='/register_user_handler' method='POST'>
                        <fieldset>
                            <label for='username' >Username:</label>
                            <input type='text' name='username' id='username' maxlength='30' size="30"/>
                            <button type='button' className="pure-button pure-button-secondary" onClick={()=> this.props.history.push("/menu")}>Back</button>
                            <button type="submit" className="pure-button pure-button-primary">Save</button>
                        </fieldset>
                    </form>
                </div>
            </>
        );
    }
}

export default RegisterUser;