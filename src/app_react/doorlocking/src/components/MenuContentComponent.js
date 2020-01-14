import React, { Component } from "react";

class MenuContent extends Component {

    handleClick(button, route){
        button.className = 'pure-button button-xlarge-fixed';
        this.props.history.push(route);
    }

    render(){
        return(
            <div className="content">
                <br/>
                <p className="center"><button onClick={() => this.handleClick(this, '/open_door')} className="button-success pure-button button-xlarge-fixed">Open Door</button></p>
                <p className="center"><button onClick={() => this.handleClick(this, '/register_user')} className="button-success pure-button button-xlarge-fixed">New User</button></p>
                <p className="center"><button onClick={() => this.handleClick(this, '/list_users')}  className="button-success pure-button button-xlarge-fixed">List/Change Users</button></p>
                <p className="center"><button onClick={() => this.handleClick(this, '/logout_handler')} className="button-success pure-button button-xlarge-fixed">Logout</button></p>
            </div>
        );
    }
}

export default MenuContent;